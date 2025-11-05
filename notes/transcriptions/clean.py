import re
from pathlib import Path

# input and output filenames
input_file = "6.1.txt"
output_file = "6.1_clean.txt"

# regex to match timestamps like:
# [mm:ss.mmm --> mm:ss.mmm]
# [hh:mm:ss.mmm --> hh:mm:ss.mmm]
timestamp_pattern = re.compile(
    r"^\[\d{2}:(?:\d{2}:)?\d{2}\.\d{3}\s*-->\s*\d{2}:(?:\d{2}:)?\d{2}\.\d{3}\]\s*"
)

with open(input_file, "r", encoding="utf-8") as fin, \
     open(output_file, "w", encoding="utf-8") as fout:
    for line in fin:
        cleaned = timestamp_pattern.sub("", line)
        fout.write(cleaned)

print(f"✅ Done. Cleaned file saved as: {output_file}")

