import shutil
import subprocess
from pathlib import Path

# Paths
root = Path(__file__).parent.resolve()
nob_exe = root / "nob.exe"
nob_c = root / "nob.c"
src_folder = root / "src" / "UniGraphics"
uni_folder = root / "UniGraphics"
include_folder = uni_folder / "include"
lib_folder = uni_folder / "lib"
build_lib = root / "build" / "libUniGraphics.a"

# Ensure UniGraphics folders exist
include_folder.mkdir(parents=True, exist_ok=True)
lib_folder.mkdir(parents=True, exist_ok=True)

# Step 1: Make sure nob.exe exists
if not nob_exe.exists():
    print("nob.exe not found, compiling...")
    result = subprocess.run(["gcc", "-o", "nob", "nob.c"], cwd=root)
    if result.returncode != 0:
        _ = input("Compilation failed! Press Enter to exit...")
        exit(1)
    print("Compiled nob.exe successfully.")

# Step 2: Run nob.exe
print("Running nob.exe...")
result = subprocess.run([str(nob_exe)], cwd=root)
if result.returncode != 0:
    _ = input("nob.exe failed! Press Enter to exit...")
    exit(1)
print("nob.exe ran successfully.")

# Step 3: Copy headers only, keep parent UniGraphics folder
dest = include_folder / "UniGraphics"

if dest.exists():
    shutil.rmtree(dest)

print(f"Copying headers from {src_folder} to {dest}")
_ = shutil.copytree(src_folder, dest)

# Remove all .cpp files
for path in dest.rglob("*.cpp"):
    print(f"Removing {path}")
    path.unlink()

# Keep .h and .hpp only
for path in dest.rglob("*"):
    if path.is_file() and path.suffix not in (".h", ".hpp"):
        print(f"Removing non-header file {path}")
        path.unlink()

# Step 4: Copy library file
if not build_lib.exists():
    _ = input(f"{build_lib} not found! Press Enter to exit...")
    exit(1)

_ = shutil.copy2(build_lib, lib_folder)
print(f"Copied {build_lib.name} to {lib_folder}")

print("Done!")
_ = input("The Library created! Press Enter to exit...")
