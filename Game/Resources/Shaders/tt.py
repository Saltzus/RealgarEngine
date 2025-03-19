import struct

# Read SPIR-V binary file
with open("test.vert.spv", "rb") as f:
    data = f.read()

# Convert binary data to 32-bit words (little-endian)
words = struct.unpack(f"{len(data) // 4}I", data)

# Format as hex and print with newlines every 8 values
for i in range(0, len(words), 8):
    print(",".join(f"0x{word:08X}" for word in words[i:i+8]) + ",")
