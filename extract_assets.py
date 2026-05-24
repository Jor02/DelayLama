import sys
import struct
from pathlib import Path

import pefile

RESOURCE_MAP = {
    130: "background.bmp",
    131: "monk_sprites.bmp",
    141: "handle_y.bmp",
    142: "handle_x.bmp",
    151: "handle_delay.bmp",
    152: "knob_glide.bmp",
    153: "knob_voice.bmp",
    160: "splash.bmp",
}

# Windows BITMAP resource type constant
RT_BITMAP = 2

# Bitmap header
BITMAPFILEHEADER_SIZE = 14
BITMAPFILEHEADER_SIGNATURE = b"BM"


def build_bmp_file_header(data: bytes) -> bytes:
    info_header_size = struct.unpack_from("<I", data, 0)[0]
    bit_count = struct.unpack_from("<H", data, 14)[0]
    colors_used = struct.unpack_from("<I", data, 32)[0]
    if colors_used == 0 and bit_count <= 8:
        colors_used = 1 << bit_count

    palette_size = colors_used * 4
    pixel_data_offset = BITMAPFILEHEADER_SIZE + info_header_size + palette_size
    file_size = BITMAPFILEHEADER_SIZE + len(data)

    header = struct.pack(
        "<2sIHHI",
        BITMAPFILEHEADER_SIGNATURE,
        file_size,
        0,
        0,
        pixel_data_offset,
    )
    return header


def extract(dll_path: Path, output_dir: Path) -> None:
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

            filename = RESOURCE_MAP[rid]

            lang_entry = resource_id_entry.directory.entries[0]
            data_entry = lang_entry.data
            rva        = data_entry.struct.OffsetToData
            size       = data_entry.struct.Size
            raw        = pe.get_data(rva, size)

            file_header = build_bmp_file_header(raw)
            bmp_bytes   = file_header + raw

            out_path = output_dir / filename
            out_path.write_bytes(bmp_bytes)

            print(f"  [{rid}] → {out_path}  ({len(bmp_bytes):,} bytes)")
            extracted[rid] = filename

    pe.close()

    missing = set(RESOURCE_MAP.keys()) - set(extracted.keys())
    if missing:
        print("\nWARNING: The following resource IDs were not found in the DLL:")
        for rid in sorted(missing):
            print(f"  {rid} ({RESOURCE_MAP[rid]})")
        sys.exit(1)

    print(f"\nDone. {len(extracted)} bitmap(s) written to '{output_dir}'.")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print(__doc__)
        sys.exit(1)

    dll  = Path(sys.argv[1])
    out  = Path(sys.argv[2]) if len(sys.argv) > 2 else Path("assets")

    if not dll.is_file():
        print(f"ERROR: DLL not found: {dll}")
        sys.exit(1)

    extract(dll, out)