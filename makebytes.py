def padded_hex(i, l):
    given_int = i
    given_len = l

    hex_result = hex(given_int)[2:] # remove '0x' from beginning of str
    num_hex_chars = len(hex_result)
    extra_zeros = '0' * (given_len - num_hex_chars) # may not get used..

    return ('0x' + hex_result if num_hex_chars == given_len else
            '?' * given_len if num_hex_chars > given_len else
            '0x' + hex_result + extra_zeros if num_hex_chars < given_len else
            None)


f = open("casm.casm", "r")
x = f.read().replace('\n','').split(',')
#print(x)
y = []
for i in x:
    if str.isdigit(i):
        p = (int(i).to_bytes(8, byteorder='little'))
        for i in p:
            y.append(int(i))
    elif i == "$RAX":
        y.append(0)
    elif i == "$RBX":
        y.append(1)
    elif i == "$RCX":
        y.append(2)
    elif i == "$RDX":
        y.append(3)
    elif i == "$RSX":
        y.append(4)
    elif i == "mov_reg_u64":
        y.append(1)
    elif i == "mov_reg_reg":
        y.append(2)
    elif i == "mov_reg_mem":
        y.append(3)
    elif i == "mov_mem_u64":
        y.append(4)
    elif i == "mov_mem_reg":
        y.append(5)
    elif i == "push":
        y.append(6)
    elif i == "pop":
        y.append(7)
    elif i == "syscall":
        y.append(8)
    elif i == "jmpz":
        y.append(9)
    elif i == "jmpnz":
        y.append(10)
    elif i == "cmp":
        y.append(11)
    elif i == "end":
        y.append(255)

    if i == "==":
        y.append(0)
    elif i == "<":
        y.append(1)
    elif i == ">":
        y.append(2)
    elif i == "<=":
        y.append(3)
    elif i == ">=":
        y.append(4)
print(y)