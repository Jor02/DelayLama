"""Regenerates docs/function_map.json from the reccmp annotations in src/.

Every function in the original DLL is annotated in the source with
`// FUNCTION: DELAYLAMA 0x...` (or LIBRARY / STUB) directly above it. This
script collects them into {"Class": {"method": "address"}}; free functions are
listed at the top level. A name that occurs at several addresses gets a list.

Usage: python tools/generate_function_map.py
"""
import json
import pathlib
import re

ROOT = pathlib.Path(__file__).resolve().parent.parent
ANNOTATION = re.compile(r"//\s*(FUNCTION|LIBRARY|STUB):\s*DELAYLAMA\s+0x([0-9a-fA-F]+)")
# A function name (optionally Class::name or ~name) followed by "("
NAME = re.compile(r"((?:\w+::)*~?\w+)\s*\(")
# Macros that look like calls in a declaration
MACROS = {"STDMETHODIMP_", "STDMETHOD", "STDMETHOD_", "__declspec"}


def function_name(lines, start):
    """Returns the qualified name of the function declared from lines[start] on."""
    text = ""
    for line in lines[start:start + 4]:
        stripped = line.strip()
        if not stripped or stripped.startswith("//"):
            continue
        text += " " + stripped
        if any(m.group(1) not in MACROS for m in NAME.finditer(text)):
            break
    for match in NAME.finditer(text):
        if match.group(1) not in MACROS:
            return match.group(1)
    return None


def main():
    functions = {}
    for path in sorted((ROOT / "src").rglob("*.cpp")):
        lines = path.read_text(encoding="utf-8", errors="replace").splitlines()
        for i, line in enumerate(lines):
            annotation = ANNOTATION.search(line)
            if not annotation:
                continue
            name = function_name(lines, i + 1)
            if not name:
                continue
            address = annotation.group(2).lower()
            parts = name.split("::")
            cls, method = (parts[-2], parts[-1]) if len(parts) > 1 else (None, parts[-1])
            table = functions.setdefault(cls, {}) if cls else functions
            if method in table and table[method] != address:
                existing = table[method] if isinstance(table[method], list) else [table[method]]
                if address not in existing:
                    existing.append(address)
                table[method] = existing
            else:
                table[method] = address

    def sort(value):
        return {k: sort(v) for k, v in sorted(value.items(), key=lambda kv: (isinstance(kv[1], dict), kv[0]))} \
            if isinstance(value, dict) else value

    out = ROOT / "docs" / "function_map.json"
    out.write_text(json.dumps(sort(functions), indent=2) + "\n", encoding="utf-8")
    count = sum(len(v) if isinstance(v, dict) else 1 for v in functions.values())
    print(f"wrote {out.relative_to(ROOT)}: {count} functions")


if __name__ == "__main__":
    main()
