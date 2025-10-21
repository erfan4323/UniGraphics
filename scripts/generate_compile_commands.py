import json
import re
from pathlib import Path

# === PATH SETUP ===
# Get current script directory (e.g. /project/scripts)
script_dir = Path(__file__).resolve().parent

# Project root (one level up, e.g. /project)
project_dir = script_dir.parent

# Log file path (e.g. /project/temp/log.txt)
log_file = project_dir / "temp" / "log.txt"

# Output file path (compile_commands.json in project root)
output_path = project_dir / "compile_commands.json"

# === READ LOG FILE ===
if not log_file.exists():
    raise FileNotFoundError(f"Log file not found: {log_file}")

with open(log_file, "r", encoding="utf-8") as f:
    log_input = f.read()

# === PARSE COMMANDS ===
compile_commands = []

for line in log_input.strip().splitlines():
    if not line.strip().startswith("[INFO] CMD:"):
        continue

    cmd = line.split("[INFO] CMD:")[1].strip()

    # skip archive / linking only commands
    if cmd.startswith("ar "):
        continue
    if not cmd.startswith("g++"):
        continue

    # find .cpp source file
    match = re.search(r"\s([^\s]+\.cpp)\s", cmd)
    if not match:
        continue

    src_file = match.group(1)
    compile_commands.append(
        {"directory": str(project_dir), "command": cmd, "file": src_file}
    )

# === WRITE OUTPUT ===
with open(output_path, "w", encoding="utf-8") as f:
    json.dump(compile_commands, f, indent=2)

print(
    f"Generated '{output_path.name}' with {len(compile_commands)} entries in:\n{project_dir}"
)
