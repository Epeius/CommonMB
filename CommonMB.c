#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>

char Maze[32] = {' ', ' ', ' ', '#', '#', '#', '#', '#', ' ', '#', ' ',
                 '#', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', '#',
                 ' ', 'E', 'S', '#', '#', '#', '#', '#', '#', '#'};
uint16_t const crc16_table[256] = {
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241, 0xC601,
    0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440, 0xCC01, 0x0CC0,
    0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40, 0x0A00, 0xCAC1, 0xCB81,
    0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841, 0xD801, 0x18C0, 0x1980, 0xD941,
    0x1B00, 0xDBC1, 0xDA81, 0x1A40, 0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01,
    0x1DC0, 0x1C80, 0xDC41, 0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0,
    0x1680, 0xD641, 0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081,
    0x1040, 0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441, 0x3C00,
    0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41, 0xFA01, 0x3AC0,
    0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840, 0x2800, 0xE8C1, 0xE981,
    0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41, 0xEE01, 0x2EC0, 0x2F80, 0xEF41,
    0x2D00, 0xEDC1, 0xEC81, 0x2C40, 0xE401, 0x24C0, 0x2580, 0xE541, 0x2700,
    0xE7C1, 0xE681, 0x2640, 0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0,
    0x2080, 0xE041, 0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281,
    0x6240, 0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41, 0xAA01,
    0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840, 0x7800, 0xB8C1,
    0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41, 0xBE01, 0x7EC0, 0x7F80,
    0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40, 0xB401, 0x74C0, 0x7580, 0xB541,
    0x7700, 0xB7C1, 0xB681, 0x7640, 0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101,
    0x71C0, 0x7080, 0xB041, 0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0,
    0x5280, 0x9241, 0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481,
    0x5440, 0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841, 0x8801,
    0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40, 0x4E00, 0x8EC1,
    0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41, 0x4400, 0x84C1, 0x8581,
    0x4540, 0x8701, 0x47C0, 0x4680, 0x8641, 0x8201, 0x42C0, 0x4380, 0x8341,
    0x4100, 0x81C1, 0x8081, 0x4040};
uint16_t crc16_byte(uint16_t crc, uint8_t data) {
  return (crc >> 8) ^ crc16_table[(crc ^ data) & 0xff];
}
uint16_t crc16(uint16_t crc, uint8_t *buffer, uint32_t len) {
  while (len--)
    crc = crc16_byte(crc, *buffer++);
  return crc;
}

int Compare16(uint8_t *buf, uint32_t size) {
  int result;

  result = 1;
  if (size >= 2) {
    result = 0;
    if (*(uint16_t *)buf == 0xDEAF) {
      assert(0);
    }
  }
  return result;
}

int Compare32(uint8_t *buf, uint32_t size) {
  int result;

  result = 1;
  if (size >= 4) {
    result = 0;
    if (*(uint32_t *)buf == 0xCADEBABA) {
      assert(0);
    }
  }
  return result;
}

int SimpleSum16(uint8_t *buf, uint32_t size) {
  int result;

  result = 1;
  if (size >= 6) {
    result = 0;
    if (*(uint16_t *)buf + 0xDEAF ==
        *((uint16_t *)buf + 1) + *((uint16_t *)buf + 2)) {
      assert(0);
    }
  }
  return result;
}

int SimpleSum32(uint8_t *buf, uint32_t size) {
  int result;

  result = 1;
  if (size >= 0xC) {
    result = 0;
    if (*(uint32_t *)buf + 0xCADEBABA ==
        *((uint32_t *)buf + 1) + *((uint32_t *)buf + 2)) {
      assert(0);
    }
  }
  return result;
}

int Complex(uint8_t *buf, uint32_t size) {
  signed int result;
  int v0;
  int v1;
  int v2;

  result = 1;
  if (size >= 0xC) {
    v0 = *(uint32_t *)buf;
    v1 = *((uint32_t *)buf + 1);
    v2 = *((uint32_t *)buf + 2);
    result = 0;
    if (3 * *(uint32_t *)buf + 2 * v1 - v2 == 1 &&
        2 * v0 - 2 * v1 + 4 * v2 == -2 && v1 / 2 - v0 == v2) {
      assert(0);
    }
  }
  return result;
}

int CheckCrc16(uint8_t *buf, uint32_t size) {
  uint16_t crc1, crc2;
  if ((uint16_t)crc16(crc1, buf, size >> 1) == 0xDEAF &&
      (uint16_t)crc16(crc2, &buf[size >> 1], size - (size >> 1)) == 0xC0DE) {
    assert(0);
  }
  return 0;
}

int CheckCrc32(uint8_t *buf, uint32_t size) {
  uint32_t crc;
  if (crc32(crc, buf, size) == 0xCADEBABA) {
    assert(0);
  }
  return 0;
}

int SolveMaze(uint8_t *buf, uint32_t size) {
  uint32_t cur;
  int x_pos;
  int y_pos;
  int cur_pos;

  if (size) {
    cur = 0;
    x_pos = 3;
    y_pos = 0;
    do {
      switch (buf[cur]) {
      case 'D':
        x_pos += x_pos < 3;
        break;
      case 'L':
        y_pos -= y_pos > 0;
        break;
      case 'R':
        y_pos += y_pos < 7;
        break;
      case 'U':
        x_pos -= x_pos > 0;
        break;
      default:
        break;
      }
      cur_pos = Maze[8 * x_pos + y_pos];
      if (cur_pos == 'E') {
        assert(0);
      } else if (cur_pos == '#') {
        return 0;
      }
      ++cur;
    } while (cur < size);
  }
  return 0;
}

int SimpleSemiSudoku(uint8_t *buf, uint32_t size) {
  int i, j, k;
  int count = 0;
  int result;
  result = 0;
  if (size >= 0x51) {
    for (i = 0; i < 81; i = i + 9) {
      int sumRow = 0;
      for (j = i; j < i + 9; j++) {
        sumRow = sumRow + buf[j];
      }
      if (sumRow != 45) {
        count++;
      }
    }

    for (i = 0; i < 9; i++) {
      int sumCol = 0;
      for (j = i; j < 81; j = j + 9) {
        sumCol = sumCol + buf[j];
      }
      if (sumCol != 45) {
        count++;
      }
    }
    if (count == 0) {
      assert(0);
      result = 1;
      return result;
    }
  }

  return result;
}

int TestOneInput(uint8_t *buf, uint32_t size) {
  if (size >= 4 && *(uint32_t *)buf == 0x5A4D4550) {
    Compare16(buf + 4, size - 4);
    SimpleSum16(buf + 4, size - 4);
    CheckCrc16(buf + 4, size - 4);
    Compare32(buf + 4, size - 4);
    SimpleSum32(buf + 4, size - 4);
    Complex(buf + 4, size - 4);
    CheckCrc32(buf + 4, size - 4);
    SolveMaze(buf + 4, size - 4);
    SimpleSemiSudoku(buf + 4, size - 4);
  }
  return 0;
}

int main(int argc, const char **argv) {
  FILE *f;
  uint32_t size;
  int result;
  char buf[0x400C];

  if (argc == 2) {
    f = fopen(argv[1], "rb");
    if (f) {
      fseek(f, 0, 2);
      size = ftell(f);
      fseek(f, 0, 0);
      if (size < 0x4001) {
        if (size == fread(buf, 1u, size, f)) {
          TestOneInput(buf, size);
          puts("Seems OK!");
          fclose(f);
          result = 0;
        } else {
          fclose(f);
          result = 4;
        }
      } else {
        fclose(f);
        result = 3;
      }
    } else {
      puts("Cannot open input file.");
      result = 2;
    }
  } else {
    printf("Usage: %s <input file>.\n", *argv);
    result = 1;
  }

  return result;
}
