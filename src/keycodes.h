#ifndef KEYCODES_H
#define KEYCODES_H

// HID Usages for a mouse.

// HID Usages for a keyboard.
#define KEYBD_NONE              (0x00)
#define KEYBD_ERR_ROLL_OVER     (0x01)
#define KEYBD_POST_FAIL         (0x02)
#define KEYBD_ERR_UNDEFINED     (0x03)
#define KEYBD_A                 (0x04)
#define KEYBD_B                 (0x05)
#define KEYBD_C                 (0x06)
#define KEYBD_D                 (0x07)
#define KEYBD_E                 (0x08)
#define KEYBD_F                 (0x09)
#define KEYBD_G                 (0x0A)
#define KEYBD_H                 (0x0B)
#define KEYBD_I                 (0x0C)
#define KEYBD_J                 (0x0D)
#define KEYBD_K                 (0x0E)
#define KEYBD_L                 (0x0F)
#define KEYBD_M                 (0x10)
#define KEYBD_N                 (0x11)
#define KEYBD_O                 (0x12)
#define KEYBD_P                 (0x13)
#define KEYBD_Q                 (0x14)
#define KEYBD_R                 (0x15)
#define KEYBD_S                 (0x16)
#define KEYBD_T                 (0x17)
#define KEYBD_U                 (0x18)
#define KEYBD_V                 (0x19)
#define KEYBD_W                 (0x1A)
#define KEYBD_X                 (0x1B)
#define KEYBD_Y                 (0x1C)
#define KEYBD_Z                 (0x1D)
#define KEYBD_1                 (0x1E)
#define KEYBD_2                 (0x1F)
#define KEYBD_3                 (0x20)
#define KEYBD_4                 (0x21)
#define KEYBD_5                 (0x22)
#define KEYBD_6                 (0x23)
#define KEYBD_7                 (0x24)
#define KEYBD_8                 (0x25)
#define KEYBD_9                 (0x26)
#define KEYBD_0                 (0x27)
#define KEYBD_RETURN            (0x28)
#define KEYBD_ENTER             (0x29)
#define KEYBD_DELETE            (0x2A)
#define KEYBD_TAB               (0x2B)
#define KEYBD_SPACE             (0x2C)
#define KEYBD_MINUS             (0x2D)
#define KEYBD_EQUALS            (0x2E)
#define KEYBD_SQUARE_L          (0x2F)
#define KEYBD_SQUARE_R          (0x30)
#define KEYBD_BACK_SLASH        (0x31)
#define KEYBD_HASH              (0x32)
#define KEYBD_SEMICOLON         (0x33)
#define KEYBD_QUOTE             (0x34)
#define KEYBD_GRAVE             (0x35)
#define KEYBD_COMMA             (0x36)
#define KEYBD_DOT               (0x37)
#define KEYBD_FORWARD_SLASH     (0x38)
#define KEYBD_CAPS_LOCK         (0x39)
#define KEYBD_F1                (0x3A)
#define KEYBD_F2                (0x3B)
#define KEYBD_F3                (0x3C)
#define KEYBD_F4                (0x3D)
#define KEYBD_F5                (0x3E)
#define KEYBD_F6                (0x3F)
#define KEYBD_F7                (0x40)
#define KEYBD_F8                (0x41)
#define KEYBD_F9                (0x42)
#define KEYBD_F10               (0x43)
#define KEYBD_F11               (0x44)
#define KEYBD_F12               (0x45)
#define KEYBD_PRINT_SCREEN      (0x46)
#define KEYBD_SCROLL_LOCK       (0x47)
#define KEYBD_PAUSE             (0x48)
#define KEYBD_INSERT            (0x49)
#define KEYBD_HOME              (0x4A)
#define KEYBD_PAGE_UP           (0x4B)
#define KEYBD_DELETE_FORWARD    (0x4C)
#define KEYBD_END               (0x4D)
#define KEYBD_PAGE_DOWN         (0x4E)
#define KEYBD_RIGHT_ARROW       (0x4F)
#define KEYBD_LEFT_ARROW        (0x50)
#define KEYBD_DOWN_ARROW        (0x51)
#define KEYBD_UP_ARROW          (0x52)

#define KEYBD_PAD_NUM_LOCK      (0x53)
#define KEYBD_PAD_DIVIDE        (0x54)
#define KEYBD_PAD_MULTIPLY      (0x55)
#define KEYBD_PAD_MINUS         (0x56)
#define KEYBD_PAD_PLUS          (0x57)
#define KEYBD_PAD_ENTER         (0x58)
#define KEYBD_PAD_1             (0x59)
#define KEYBD_PAD_2             (0x5A)
#define KEYBD_PAD_3             (0x5B)
#define KEYBD_PAD_4             (0x5C)
#define KEYBD_PAD_5             (0x5D)
#define KEYBD_PAD_6             (0x5E)
#define KEYBD_PAD_7             (0x5F)
#define KEYBD_PAD_8             (0x60)
#define KEYBD_PAD_9             (0x61)
#define KEYBD_PAD_0             (0x62)
#define KEYBD_PAD_DOT           (0x63)

#define KEYBD_APPLICATION       (0x65)
#define KEYBD_POWER             (0x66)
#define KEYBD_PAD_EQUALS        (0x67)
#define KEYBD_F13               (0x68)
#define KEYBD_F14               (0x69)
#define KEYBD_F15               (0x6A)
#define KEYBD_F16               (0x6B)
#define KEYBD_F17               (0x6C)
#define KEYBD_F18               (0x6D)
#define KEYBD_F19               (0x6E)
#define KEYBD_F20               (0x6F)
#define KEYBD_F21               (0x70)
#define KEYBD_F22               (0x71)
#define KEYBD_F23               (0x72)
#define KEYBD_F24               (0x73)
  /*
  0x64 Keyboard Non-US \ and |
  0x74 Keyboard Execute
  0x75 Keyboard Help
  0x76 Keyboard Menu
  0x77 Keyboard Select
  0x78 Keyboard Stop
  0x79 Keyboard Again
  0x7A Keyboard Undo
  0x7B Keyboard Cut
  0x7C Keyboard Copy
  0x7D Keyboard Paste
  0x7E Keyboard Find
  0x7F Keyboard Mute
  0x80 Keyboard Volume Up
  0x81 Keyboard Volume Down
  0x82 Keyboard Locking Caps Lock
  0x83 Keyboard Locking Num Lock
  0x84 Keyboard Locking Scroll Lock
  0x85 Keypad Comma
  0x86 Keypad Equal Sign
  0x87 Keyboard International1
  0x88 Keyboard International2
  0x89 Keyboard International3
  0x8A Keyboard International4
  0x8B Keyboard International5
  0x8C Keyboard International6
  0x8D Keyboard International7
  0x8E Keyboard International8
  0x8F Keyboard International9
  0x90 Keyboard LANG1
  0x91 Keyboard LANG2
  0x92 Keyboard LANG3
  0x93 Keyboard LANG4
  0x94 Keyboard LANG5
  0x95 Keyboard LANG6
  0x96 Keyboard LANG7
  0x97 Keyboard LANG8
  0x98 Keyboard LANG9
  0x99 Keyboard Alternate Erase
  0x9A Keyboard SysReq/Attention
  0x9B Keyboard Cancel
  0x9C Keyboard Clear
  0x9D Keyboard Prior
  0x9E Keyboard Return
  0x9F Keyboard Separator
  0xA0 Keyboard Out
  0xA1 Keyboard Oper
  0xA2 Keyboard Clear/Again
  0xA3 Keyboard CrSel/Props
  0xA4 Keyboard ExSel
  */
#define KEYBD_MOD_CTRL_LEFT_MASK   (1 << 0)
#define KEYBD_MOD_SHIFT_LEFT_MASK  (1 << 1)
#define KEYBD_MOD_ALT_LEFT_MASK    (1 << 2)
#define KEYBD_MOD_META_LEFT_MASK   (1 << 3)
#define KEYBD_MOD_CTRL_RIGHT_MASK  (1 << 4)
#define KEYBD_MOD_SHIFT_RIGHT_MASK (1 << 5)
#define KEYBD_MOD_ALT_RIGHT_MASK   (1 << 6)
#define KEYBD_MOD_META_RIGHT_MASK  (1 << 7)

#endif /* KEYCODES_H */

