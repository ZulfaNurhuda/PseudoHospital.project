# Makefile untuk Sistem Manajemen Rumah Sakit

# Kompiler dan flag
CC = gcc
CFLAGS = -Wall -std=c99 -Iheader -Wno-unused-function
DEBUG_FLAGS = -g

# Direktori
SRC_DIR = c
HEADER_DIR = header
BUILD_DIR = build

# Nama executable
TARGET = hospitalSystem

# Jumlah baris dan kolom denah
ROWS = 3
COLS = 3

# Daftar file sumber
SOURCES = hospitalSystem.c \
          $(SRC_DIR)/addDoctor.c \
          $(SRC_DIR)/bananarich.c \
          $(SRC_DIR)/canGoHome.c \
          $(SRC_DIR)/diagnose.c \
          $(SRC_DIR)/dynamicLayout.c \
          $(SRC_DIR)/enigma.c \
          $(SRC_DIR)/exit.c \
          $(SRC_DIR)/forgotPassword.c \
          $(SRC_DIR)/hospital.c \
          $(SRC_DIR)/hospitalLayout.c \
          $(SRC_DIR)/login.c \
          $(SRC_DIR)/logout.c \
          $(SRC_DIR)/menuHelp.c \
          $(SRC_DIR)/queuePrimitive.c \
          $(SRC_DIR)/queueManagement.c \
          $(SRC_DIR)/registerCheckup.c \
          $(SRC_DIR)/registerPatient.c \
          $(SRC_DIR)/runLengthEncoding.c \
          $(SRC_DIR)/searchUser.c \
          $(SRC_DIR)/takeAntidote.c \
          $(SRC_DIR)/takeMedication.c \
          $(SRC_DIR)/treat.c \
          $(SRC_DIR)/utils.c \
          $(SRC_DIR)/viewLifeStatus.c \
          $(SRC_DIR)/viewPatientQueue.c \
          $(SRC_DIR)/viewQueue.c \
          $(SRC_DIR)/viewUsers.c


# Daftar file objek
OBJECTS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(SOURCES)))

# Target utama: membangun executable
all: $(BUILD_DIR) $(TARGET)

# Membuat direktori build jika belum ada
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Link semua file objek menjadi executable
$(TARGET): $(OBJECTS)
	@echo "Linking ke $(TARGET)..."
	@$(CC) $(OBJECTS) -o $(TARGET)
	@echo "Build selesai!"

# Kompilasi file sumber ke file objek
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Mengompilasi $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/hospitalSystem.o: hospitalSystem.c
	@echo "Mengompilasi hospitalSystem.c..."
	@$(CC) $(CFLAGS) -c $< -o $@

# Target untuk build dengan debugging
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean $(BUILD_DIR) $(TARGET)

# Target untuk menjalankan program
run: all
	@echo "Menjalankan $(TARGET)..."
	@./$(TARGET) $(ROWS) $(COLS)

# Target untuk membersihkan file hasil build
clean:
	@echo "Membersihkan..."
	@rm -rf $(BUILD_DIR) $(TARGET)
	@echo "Pembersihan selesai!"

# Menandakan bahwa target ini bukan file
.PHONY: all debug run clean