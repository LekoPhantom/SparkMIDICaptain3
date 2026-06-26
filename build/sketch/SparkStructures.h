#line 1 "/Users/leko/Downloads/SparkMIDICaptain-main/SparkMIDICaptain3/SparkStructures.h"
#ifndef SparkStructures_h
#define SparkStructures_h

#define STR_LEN 40

// ------------------------------------------------------------
// Existing SparkPreset struct (unchanged)
// ------------------------------------------------------------
typedef struct  {
  uint8_t  curr_preset;
  uint8_t  preset_num;
  char UUID[STR_LEN];
  char Name[STR_LEN];
  char Version[STR_LEN];
  char Description[STR_LEN];
  char Icon[STR_LEN];
  float BPM;
  uint8_t num_effects;

  struct SparkEffects {
    char EffectName[STR_LEN];
    bool OnOff;
    uint8_t  NumParameters;
    float Parameters[10];
  } effects[7];

  uint8_t chksum;
} SparkPreset;


// ------------------------------------------------------------
// Existing SparkMessage struct (unchanged)
// ------------------------------------------------------------
typedef struct {
  uint8_t param1;
  uint8_t param2;
  uint8_t param3;
  uint8_t param4;
  uint8_t param5;
  uint8_t param6;
  uint8_t param7;
  uint8_t param8;
  uint32_t param10;
  float val;
  char str1[STR_LEN];
  char str2[STR_LEN];
  bool onoff;
  bool bool1;
  bool bool2;
} SparkMessage;


// ============================================================================
// ADDITIONS BEGIN HERE
// ============================================================================

// ------------------------------------------------------------
// Spark MsgPack Value Representation
// ------------------------------------------------------------
enum SparkValueType {
  SPARK_VAL_INT,
  SPARK_VAL_FLOAT,
  SPARK_VAL_STRING,
  SPARK_VAL_BOOL
};

struct SparkValue {
  SparkValueType type;
  int   i;
  float f;
  String s;
  bool  b;
};


// ------------------------------------------------------------
// Minimal MsgPack Decoder for Spark Protocol
// ------------------------------------------------------------
// NOTE: This expects the 7‑bit packing to already be undone.
//       (SparkIO.ino will call spark_unpack_7bit() first.)
//
// Handles:
//   - short strings (0xa0–0xbf)
//   - uint8 (0xcc <byte>)
//   - float32 (0xca <4 bytes>)
//   - booleans (0xc2/0xc3)
//   - fixed arrays (0x90–0x9f)
// ------------------------------------------------------------
int spark_decode_msgpack(const uint8_t* buf, int len, SparkValue* out, int maxOut) {
  int idx = 0;
  int count = 0;

  while (idx < len && count < maxOut) {
    uint8_t b = buf[idx];

    // -------------------------
    // Short string (fixstr)
    // -------------------------
    if ((b & 0xE0) == 0xA0) {
      int slen = b & 0x1F;
      idx++;
      String s;
      for (int i = 0; i < slen && idx < len; i++) {
        s += (char)buf[idx++];
      }
      out[count].type = SPARK_VAL_STRING;
      out[count].s = s;
      count++;
    }

    // -------------------------
    // Unsigned int (uint8)
    // -------------------------
    else if (b == 0xCC && idx + 1 < len) {
      idx++;
      out[count].type = SPARK_VAL_INT;
      out[count].i = buf[idx++];
      count++;
    }

    // -------------------------
    // Float32
    // -------------------------
    else if (b == 0xCA && idx + 4 < len) {
      idx++;
      uint32_t raw = ((uint32_t)buf[idx] << 24) |
                     ((uint32_t)buf[idx+1] << 16) |
                     ((uint32_t)buf[idx+2] << 8) |
                     ((uint32_t)buf[idx+3]);
      idx += 4;

      float f;
      memcpy(&f, &raw, sizeof(float));

      out[count].type = SPARK_VAL_FLOAT;
      out[count].f = f;
      count++;
    }

    // -------------------------
    // Boolean
    // -------------------------
    else if (b == 0xC2 || b == 0xC3) {
      out[count].type = SPARK_VAL_BOOL;
      out[count].b = (b == 0xC3);
      idx++;
      count++;
    }

    // -------------------------
    // Fixed array header
    // -------------------------
    else if ((b & 0xF0) == 0x90) {
      idx++; // skip header
    }

    // -------------------------
    // Unknown → stop
    // -------------------------
    else {
      break;
    }
  }

  return count;
}


// ============================================================================
// ADDITIONS END HERE
// ============================================================================

#endif
