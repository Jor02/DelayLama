import argparse
import struct
from pathlib import Path
import sys

try:
    import pefile
    PEFILE_AVAILABLE = True
except ImportError:
    PEFILE_AVAILABLE = False

# Maps resource ID -> (filename, (width, height))
RESOURCE_MAP = {
    130: ("background.bmp",   (360,  510)),
    131: ("monk_sprites.bmp", (1570, 1866)),
    141: ("handle_y.bmp",     (10,   10)),
    142: ("handle_x.bmp",     (10,   10)),
    151: ("handle_delay.bmp", (20,   17)),
    152: ("knob_glide.bmp",   (50,   3000)),
    153: ("knob_voice.bmp",   (50,   3000)),
    160: ("splash.bmp",       (253,  275)),
}

# Windows BITMAP resource type constant
RT_BITMAP = 2

# Bitmap file header
BITMAPFILEHEADER_SIZE = 14
BITMAPFILEHEADER_SIGNATURE = b"BM"

def build_bmp_file_header(data: bytes) -> bytes:
    info_header_size = struct.unpack_from("<I", data, 0)[0]
    bit_count        = struct.unpack_from("<H", data, 14)[0]
    colors_used      = struct.unpack_from("<I", data, 32)[0]
    if colors_used == 0 and bit_count <= 8:
        colors_used = 1 << bit_count

    palette_size      = colors_used * 4
    pixel_data_offset = BITMAPFILEHEADER_SIZE + info_header_size + palette_size
    file_size         = BITMAPFILEHEADER_SIZE + len(data)

    return struct.pack(
        "<2sIHHI",
        BITMAPFILEHEADER_SIGNATURE,
        file_size,
        0,
        0,
        pixel_data_offset,
    )


def build_placeholder_bmp(w: int, h: int) -> bytes:
    """Return a valid 24-bit black BMP for the given dimensions."""
    row_size   = (w * 3 + 3) & ~3
    pixel_data = bytes(row_size * h)
    file_size  = BITMAPFILEHEADER_SIZE + 40 + len(pixel_data)

    file_header = struct.pack(
        "<2sIHHI",
        BITMAPFILEHEADER_SIGNATURE,
        file_size,
        0, 0,
        BITMAPFILEHEADER_SIZE + 40,
    )
    dib_header = struct.pack(
        "<IiiHHIIiiII",
        40,
        w, -h,
        1,
        24,
        0,
        len(pixel_data),
        2835, 2835,
        0, 0,
    )
    return file_header + dib_header + pixel_data


def generate_placeholders(output_dir: Path) -> None:
    """Generate blank placeholder BMPs for every asset in RESOURCE_MAP."""
    output_dir.mkdir(parents=True, exist_ok=True)
    print("Generating placeholder bitmap assets ...")

    for rid, (filename, (w, h)) in sorted(RESOURCE_MAP.items()):
        out_path = output_dir / filename
        out_path.write_bytes(build_placeholder_bmp(w, h))
        print(f"  [{rid}] -> {out_path}  ({w}x{h}, placeholder)")

    print(f"\nDone. {len(RESOURCE_MAP)} placeholder bitmap(s) written to '{output_dir}'.")


def extract(dll_path: Path, output_dir: Path) -> None:
    """Extract real bitmap assets from the original DLL."""
    if not PEFILE_AVAILABLE:
        print("ERROR: 'pefile' is not installed. Run: pip install pefile")
        sys.exit(1)

    output_dir.mkdir(parents=True, exist_ok=True)
    print(f"Loading {dll_path} ...")
    pe = pefile.PE(str(dll_path), fast_load=False)

    if not hasattr(pe, "DIRECTORY_ENTRY_RESOURCE"):
        print("ERROR: No resource directory found in the DLL.")
        sys.exit(1)

    extracted = {}

    for resource_type in pe.DIRECTORY_ENTRY_RESOURCE.entries:
        if resource_type.id != RT_BITMAP:
            continue

        for resource_id_entry in resource_type.directory.entries:
            rid = resource_id_entry.id

            if rid not in RESOURCE_MAP:
                print(f"  Skipping unknown resource ID {rid}")
                continue

            filename, _ = RESOURCE_MAP[rid]

            lang_entry = resource_id_entry.directory.entries[0]
            data_entry = lang_entry.data
            rva        = data_entry.struct.OffsetToData
            size       = data_entry.struct.Size
            raw        = pe.get_data(rva, size)

            file_header = build_bmp_file_header(raw)
            bmp_bytes   = file_header + raw

            out_path = output_dir / filename
            out_path.write_bytes(bmp_bytes)

            print(f"  [{rid}] -> {out_path}  ({len(bmp_bytes):,} bytes)")
            extracted[rid] = filename

    pe.close()

    missing = set(RESOURCE_MAP.keys()) - set(extracted.keys())
    if missing:
        print("\nWARNING: The following resource IDs were not found in the DLL:")
        for rid in sorted(missing):
            filename, _ = RESOURCE_MAP[rid]
            print(f"  {rid} ({filename})")
        sys.exit(1)

    print(f"\nDone. {len(extracted)} bitmap(s) written to '{output_dir}'.")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Extract or generate bitmap assets for DelayLama.",
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )

    source = parser.add_mutually_exclusive_group(required=True)
    source.add_argument(
        "dll",
        nargs="?",
        type=Path,
        metavar="DLL",
        help="path to the original DelayLama.dll to extract assets from",
    )
    source.add_argument(
        "--placeholder",
        action="store_true",
        help="generate blank placeholder BMPs instead of extracting from a DLL",
    )

    parser.add_argument(
        "--output", "-o",
        type=Path,
        default=Path("assets"),
        metavar="DIR",
        help="directory to write bitmap files into (default: assets/)",
    )

    args = parser.parse_args()

    if args.dll is not None and not args.dll.is_file():
        parser.error(f"DLL not found: {args.dll}")

    return args


if __name__ == "__main__":
    args = parse_args()

    if args.placeholder:
        generate_placeholders(args.output)
    else:
        extract(args.dll, args.output)