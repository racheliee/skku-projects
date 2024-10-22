import sys

#read binary file
def read_binary_file(file_path):
        binary = []

        with open(file_path, 'rb') as binary_file:
            while True:
                line = binary_file.read(4) #read 4 bytes each 
                if not line:
                    break
                decimal_value = int.from_bytes(line, byteorder='little', signed = False) #convert to decimal
                binary_value = format(decimal_value, '032b') # convert back to binary (big endian)
                binary.append(binary_value) #add the binary into the binary_data

        return binary

#converts binary to hexadecimal
def convert_to_hex(binary):
    return hex(int(binary, 2))[2:].zfill(8) #prints without 0x and padded to match 8 digits

#converts signed binary string to decimal
def to_twos_comp(binary_str, bits):
    val = int(binary_str, 2)
    if (val & (1 << (bits - 1))) != 0: # if sign bit is = 1
        val = val - (1 << bits)        # compute negative value
    return val  

def r_format_disassemble(binary, opcode):
    #funct3 and funct 7 saves as string
    funct7 = binary[:7]
    funct3 = binary[17:20]

    #registers are converted into decimal int
    rs1 = int(binary[12:17], 2)
    rs2 = int(binary[7:12], 2)
    rd = int(binary[20:25], 2)

    assembly = '' #saves assembly instruction
    instruction = '' #saves instruction type

    if opcode == '0110011':
        if funct7 == '0000000':
            if funct3 == '000': #add
                instruction = 'add'
            elif funct3 == '001': #sll 
                instruction = 'sll'
            elif funct3 == '010': #slt 
                instruction = 'slt'
            elif funct3 == '011': #sltu
                instruction = 'sltu'
            elif funct3 == '100': #xor
                instruction = 'xor'
            elif funct3 == '101': #srl
                instruction = 'srl'
            elif funct3 == '110': #or
                instruction = 'or'
            elif funct3 == '111': #and
                instruction = 'and'
        elif funct7 == '0100000':
            if funct3 == '000': #sub
                instruction = 'sub'
            if funct3 == '101': #sra
                instruction = 'sra'
        if(instruction != ''):
            assembly = instruction + " x" + str(rd) + ", x" + str(rs1) + ", x" + str(rs2)
    elif opcode == '0010011':
        if funct7 == '0000000':
            if funct3 == '001': #slli
                instruction = 'slli'
            if funct3 == '101': #srli
                instruction = 'srli'
        if funct7 == '0100000': 
            if funct3 == '101': #srai
                instruction = 'srai'
        if(instruction != ''):
            assembly = instruction + " x" + str(rd) + ", x" + str(rs1) + ", " + str(rs2) #since rs2 = shamt

    return assembly

def i_format_disassemble(binary, opcode):
    #funct3 saved as string
    funct3 = binary[17:20]

    #immediate converted to two's complement
    immediate = to_twos_comp(binary[:12], len(binary[:12]))

    #registers saved as integers
    rs1 = int(binary[12:17], 2)
    rd = int(binary[20:25], 2)

    assembly = '' #saves assembly code
    instruction = '' #saves the instruction type

    if opcode == '0000011':
        if funct3 == '000': #lb
            instruction = 'lb'
        elif funct3 == '001': #lh
            instruction = 'lh'
        elif funct3 == '010': #lw
            instruction = 'lw'
        elif funct3 == '100': #lbu
            instruction = 'lbu'
        elif funct3 == '101': #lhu
            instruction = 'lhu'
        if(instruction != ''):
          assembly = instruction + " x" + str(rd) + ", " + str(immediate) + "(x" + str(rs1) + ")" #ex: lw x10, 0(x10)
    elif opcode == '0010011':
        if funct3 == '000': #addi
            instruction = 'addi'
        elif funct3 == '010': #slti
            instruction = 'slti'
        elif funct3 == '011': #sltiu
            instruction = 'sltiu'
        elif funct3 == '100': #xori
            instruction = 'xori'
        elif funct3 == '110': #ori
            instruction = 'ori'
        elif funct3 == '111': #andi
            instruction = 'andi'
        if(instruction != ''):
            assembly = instruction + " x" + str(rd) + ", x" + str(rs1) + ", " + str(immediate) #ex: addi x24, x24, 1
    elif opcode == '1100111':
        if funct3 == '000': #jalr
            instruction = 'jalr'
        if(instruction != ''):
            assembly = instruction + " x" + str(rd) + ", " + str(immediate) + "(x" + str(rs1) + ")" #ex: jalr x1, offset(x2)

    return assembly

def s_format_disassemble(binary):
    #funct3 saved as string
    funct3 = binary[17:20]

    #register numbers to decimal int
    rs2 = int(binary[7:12], 2)
    rs1 = int(binary[12:17], 2)
    
    #immediate converted to two's complement
    immediate = binary[:7] + binary[20:25]
    immediate = to_twos_comp(immediate, len(immediate))

    assembly = '' #saves assembly instruction
    instruction = '' # saves instruction type

    if funct3 == '000': #sb
        instruction = 'sb'
    elif funct3 == '001': #sh
        instruction = 'sh'
    elif funct3 == '010': #sw
        instruction = 'sw'
    
    if(instruction != ''):
            assembly = instruction + " x" + str(rs2) + ", " + str(immediate) + "(x" + str(rs1)+ ")"
   
    return assembly

def sb_format_disassemble(binary):
    #funct3 saved as string 
    funct3 = binary[17:20]

    #registers saved as integers
    rs2 = int(binary[7:12], 2)
    rs1 = int(binary[12:17], 2)
    
    #immediate converted to two's complement
    immediate = binary[:1] + binary[24:25] + binary[1:7]+ binary[20:24] + "0"
    immediate = to_twos_comp(immediate, len(immediate))
    
    assembly = ''
    instruction = ''

    if funct3 == '000': #beq
        instruction = 'beq'
    elif funct3 == '001': #bne
        instruction = 'bne'
    elif funct3 == '100': #blt
        instruction = 'blt'
    elif funct3 == '101': #bge
        instruction = 'bge'
    elif funct3 == '110': #bltu
        instruction = 'bltu'
    elif funct3 == '111': #bgeu
        instruction = 'bgeu'

    if(instruction != ''):
        assembly = instruction + " x" + str(rs1) + ", x" + str(rs2) + ", " + str(immediate)

    return assembly

def uj_format_disassemble(binary):
    #register saved as int
    rd = int(binary[20:25], 2)
    
    #immediate converted to two's complement
    immediate = binary[0] + binary[12:20] + binary[11] + binary[1:11] + "0"
    immediate = to_twos_comp(immediate, len(immediate))

    assembly = 'jal x' + str(rd) + ", " + str(immediate)

    return assembly

def u_format_disassemble(binary, opcode):
    #register saved as int
    rd = int(binary[20:25], 2)

    #immediate converted to two's complement
    immediate = binary[:20] + '000000000000'
    #print(immediate)
    immediate = to_twos_comp(immediate, len(immediate))

    assembly = ''

    if opcode == '0110111':
        assembly = 'lui x' + str(rd) + ', ' + str(immediate)
    elif opcode == '0010111':
        assembly = 'auipc x' + str(rd) + ', ' + str(immediate)

    return assembly

def disassemble(binary_line, opcode):
    assembly_code = ''

     # r-format disassemble
    if opcode == "0110011" or opcode == "0010011":
        assembly_code = r_format_disassemble(binary_line, opcode) 
        #print(assembly_code)   
        if(assembly_code != ''):
            return assembly_code
        
    # i-format diassemble
    if opcode == "0000011" or opcode == "0010011" or opcode == "1100111":
        assembly_code = i_format_disassemble(binary_line, opcode)
        #print('i')
        if(assembly_code != ''):
            return assembly_code
        
    # s-format disassemble
    if opcode == "0100011":
        assembly_code = s_format_disassemble(binary_line)
        if(assembly_code != ''):
            return assembly_code
        
    # sb-format disassemble
    if opcode == "1100011":
        assembly_code = sb_format_disassemble(binary_line)
        if(assembly_code != ''):
            return assembly_code
        
    # uj-format disassemble
    if opcode == "1101111":
        assembly_code = uj_format_disassemble(binary_line)
        if(assembly_code != ''):
            return assembly_code
        
    # u-format disassemble
    if opcode == "0110111" or opcode == "0010111":
        assembly_code = u_format_disassemble(binary_line, opcode)
        if(assembly_code != ''):
            return assembly_code
    
    return assembly_code

#main ========================================================

file_name = sys.argv[1]
binary_data = read_binary_file(file_name)

count = 0 #count the number of instructions

#loop through each binary_line elem in binary_data
for binary_line in binary_data:
    hexa = convert_to_hex(binary_line) # converting the binary into hexa

    #extracting opcode
    opcode = binary_line[25:]

    assembly_code = disassemble(binary_line, opcode)
   
    #in case the opcode is correct, but funct is not, assembly code will be empty
    #print unknown instruction
    if assembly_code == "":
        print("inst " + str(count) + ": " + hexa + " unknown instruction")
        count += 1

    #if the instruction is NOT unknown and the string is NOT empty, print assembly instruction
    if assembly_code != "":
        print("inst " + str(count) + ": " + hexa + " " + assembly_code)
        count += 1
