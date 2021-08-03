with open('/home/jas/Documents/dev/adorad/test/LexerDemo.ad', 'r') as f:
    s = f.read()

for i in range(50):
    s += s

print(f'Length = {len(s)}')

with open('/home/jas/Documents/dev/adorad/test/LexerDemo.ad', 'w') as f:
    f.write(s)