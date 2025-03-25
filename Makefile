# Definir nombre del proyecto
PRJ_NAME = TM4C_Maestro

# Definir herramientas de compilación
CC = arm-none-eabi-gcc
SIZE = arm-none-eabi-size
OBJDUMP = arm-none-eabi-objdump
OBJCOPY = arm-none-eabi-objcopy



# Definir opciones de compilación para debugging en Ozone
CFLAGS = -mcpu=cortex-m4 -mthumb -std=gnu11 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -IInc -Idrivers -g3 -ggdb -Og -ffunction-sections -fdata-sections
LDFLAGS = -mcpu=cortex-m4 -T startup/tm4c123gh6pm.ld --specs=nosys.specs -Wl,-Map=$(BUILD_DIR)/$(PRJ_NAME).map -static -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group

# Directorios
SRC_DIR = src
INC_DIR = Inc  
STARTUP_DIR = startup
BUILD_DIR = build
OUTPUT_DIR = $(BUILD_DIR)/output

# Archivos fuente y objetos
SRCS = $(wildcard $(SRC_DIR)/*.c)
STARTUP_SRC = $(STARTUP_DIR)/tm4c123gh6pm_startup_ccs_gcc.c
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
STARTUP_OBJ = $(BUILD_DIR)/tm4c123gh6pm_startup_ccs_gcc.o
ELF = $(BUILD_DIR)/$(PRJ_NAME).elf
HEX = $(OUTPUT_DIR)/$(PRJ_NAME).hex
BIN = $(OUTPUT_DIR)/$(PRJ_NAME).bin
LIST = $(BUILD_DIR)/$(PRJ_NAME).list
LST = $(BUILD_DIR)/$(PRJ_NAME).elf.lst

# Regla principal
all: $(HEX) $(BIN) $(LIST) $(LST) size

# Compilar archivos fuente en C
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compilar startup en ensamblador
$(STARTUP_OBJ): $(STARTUP_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

# Generar el ELF con símbolos de depuración
$(ELF): $(OBJS) $(STARTUP_OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# Generar el archivo .hex
$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

# Generar el archivo .bin (para flasheo rápido)
$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

# Generar el archivo .list (desensamblado)
$(LIST): $(ELF)
	$(OBJDUMP) -h -S $< > $@

# Generar el archivo .elf.lst con información detallada
$(LST): $(ELF)
	$(OBJDUMP) -d -S $< > $@

# Mostrar tamaño del ejecutable
size: $(ELF)
	$(SIZE) $<

# Limpiar archivos generados
clean:
	rm -rf $(BUILD_DIR)/*.o $(ELF) $(HEX) $(BIN) $(LIST) $(LST) $(BUILD_DIR)/*.map $(OUTPUT_DIR)/*
	