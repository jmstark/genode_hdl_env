static const char *key_strings[] = {
	"KEY_RESERVED",
	"KEY_ESC",
	"KEY_1",
	"KEY_2",
	"KEY_3",
	"KEY_4",
	"KEY_5",
	"KEY_6",
	"KEY_7",
	"KEY_8",
	"KEY_9",
	"KEY_0",
	"KEY_MINUS",
	"KEY_EQUAL",
	"KEY_BACKSPACE",
	"KEY_TAB",
	"KEY_Q",
	"KEY_W",
	"KEY_E",
	"KEY_R",
	"KEY_T",
	"KEY_Y",
	"KEY_U",
	"KEY_I",
	"KEY_O",
	"KEY_P",
	"KEY_LEFTBRACE",
	"KEY_RIGHTBRACE",
	"KEY_ENTER",
	"KEY_LEFTCTRL",
	"KEY_A",
	"KEY_S",
	"KEY_D",
	"KEY_F",
	"KEY_G",
	"KEY_H",
	"KEY_J",
	"KEY_K",
	"KEY_L",
	"KEY_SEMICOLON",
	"KEY_APOSTROPHE",
	"KEY_GRAVE",
	"KEY_LEFTSHIFT",
	"KEY_BACKSLASH",
	"KEY_Z",
	"KEY_X",
	"KEY_C",
	"KEY_V",
	"KEY_B",
	"KEY_N",
	"KEY_M",
	"KEY_COMMA",
	"KEY_DOT",
	"KEY_SLASH",
	"KEY_RIGHTSHIFT",
	"KEY_KPASTERISK",
	"KEY_LEFTALT",
	"KEY_SPACE",
	"KEY_CAPSLOCK",
	"KEY_F1",
	"KEY_F2",
	"KEY_F3",
	"KEY_F4",
	"KEY_F5",
	"KEY_F6",
	"KEY_F7",
	"KEY_F8",
	"KEY_F9",
	"KEY_F10",
	"KEY_NUMLOCK",
	"KEY_SCROLLLOCK",
	"KEY_KP7",
	"KEY_KP8",
	"KEY_KP9",
	"KEY_KPMINUS",
	"KEY_KP4",
	"KEY_KP5",
	"KEY_KP6",
	"KEY_KPPLUS",
	"KEY_KP1",
	"KEY_KP2",
	"KEY_KP3",
	"KEY_KP0",
	"KEY_KPDOT",
	"unused_84",
	"KEY_ZENKAKUHANKAKU",
	"KEY_102ND",
	"KEY_F11",
	"KEY_F12",
	"KEY_RO",
	"KEY_KATAKANA",
	"KEY_HIRAGANA",
	"KEY_HENKAN",
	"KEY_KATAKANAHIRAGANA",
	"KEY_MUHENKAN",
	"KEY_KPJPCOMMA",
	"KEY_KPENTER",
	"KEY_RIGHTCTRL",
	"KEY_KPSLASH",
	"KEY_SYSRQ",
	"KEY_RIGHTALT",
	"KEY_LINEFEED",
	"KEY_HOME",
	"KEY_UP",
	"KEY_PAGEUP",
	"KEY_LEFT",
	"KEY_RIGHT",
	"KEY_END",
	"KEY_DOWN",
	"KEY_PAGEDOWN",
	"KEY_INSERT",
	"KEY_DELETE",
	"KEY_MACRO",
	"KEY_MUTE",
	"KEY_VOLUMEDOWN",
	"KEY_VOLUMEUP",
	"KEY_POWER",
	"KEY_KPEQUAL",
	"KEY_KPPLUSMINUS",
	"KEY_PAUSE",
	"unused_120",
	"KEY_KPCOMMA",
	"KEY_HANGUEL",
	"KEY_HANJA",
	"KEY_YEN",
	"KEY_LEFTMETA",
	"KEY_RIGHTMETA",
	"KEY_COMPOSE",
	"KEY_STOP",
	"KEY_AGAIN",
	"KEY_PROPS",
	"KEY_UNDO",
	"KEY_FRONT",
	"KEY_COPY",
	"KEY_OPEN",
	"KEY_PASTE",
	"KEY_FIND",
	"KEY_CUT",
	"KEY_HELP",
	"KEY_MENU",
	"KEY_CALC",
	"KEY_SETUP",
	"KEY_SLEEP",
	"KEY_WAKEUP",
	"KEY_FILE",
	"KEY_SENDFILE",
	"KEY_DELETEFILE",
	"KEY_XFER",
	"KEY_PROG1",
	"KEY_PROG2",
	"KEY_WWW",
	"KEY_MSDOS",
	"KEY_COFFEE",
	"KEY_DIRECTION",
	"KEY_CYCLEWINDOWS",
	"KEY_MAIL",
	"KEY_BOOKMARKS",
	"KEY_COMPUTER",
	"KEY_BACK",
	"KEY_FORWARD",
	"KEY_CLOSECD",
	"KEY_EJECTCD",
	"KEY_EJECTCLOSECD",
	"KEY_NEXTSONG",
	"KEY_PLAYPAUSE",
	"KEY_PREVIOUSSONG",
	"KEY_STOPCD",
	"KEY_RECORD",
	"KEY_REWIND",
	"KEY_PHONE",
	"KEY_ISO",
	"KEY_CONFIG",
	"KEY_HOMEPAGE",
	"KEY_REFRESH",
	"KEY_EXIT",
	"KEY_MOVE",
	"KEY_EDIT",
	"KEY_SCROLLUP",
	"KEY_SCROLLDOWN",
	"KEY_KPLEFTPAREN",
	"KEY_KPRIGHTPAREN",
	"KEY_NEW",
	"KEY_REDO",
	"KEY_F13",
	"KEY_F14",
	"KEY_F15",
	"KEY_F16",
	"KEY_F17",
	"KEY_F18",
	"KEY_F19",
	"KEY_F20",
	"KEY_F21",
	"KEY_F22",
	"KEY_F23",
	"KEY_F24",
	"unused_195",
	"unused_196",
	"unused_197",
	"unused_198",
	"unused_199",
	"KEY_PLAYCD",
	"KEY_PAUSECD",
	"KEY_PROG3",
	"KEY_PROG4",
	"KEY_DASHBOARD",
	"KEY_SUSPEND",
	"KEY_CLOSE",
	"KEY_PLAY",
	"KEY_FASTFORWARD",
	"KEY_BASSBOOST",
	"KEY_PRINT",
	"KEY_HP",
	"KEY_CAMERA",
	"KEY_SOUND",
	"KEY_QUESTION",
	"KEY_EMAIL",
	"KEY_CHAT",
	"KEY_SEARCH",
	"KEY_CONNECT",
	"KEY_FINANCE",
	"KEY_SPORT",
	"KEY_SHOP",
	"KEY_ALTERASE",
	"KEY_CANCEL",
	"KEY_BRIGHTNESSDOWN",
	"KEY_BRIGHTNESSUP",
	"KEY_MEDIA",
	"KEY_SWITCHVIDEOMODE",
	"KEY_KBDILLUMTOGGLE",
	"KEY_KBDILLUMDOWN",
	"KEY_KBDILLUMUP",
	"KEY_SEND",
	"KEY_REPLY",
	"KEY_FORWARDMAIL",
	"KEY_SAVE",
	"KEY_DOCUMENTS",
	"KEY_BATTERY",
	"KEY_BLUETOOTH",
	"KEY_WLAN",
	"unused_239",
	"unused_240",
	"unused_241",
	"unused_242",
	"unused_243",
	"unused_244",
	"unused_245",
	"unused_246",
	"unused_247",
	"unused_248",
	"unused_249",
	"unused_250",
	"unused_251",
	"unused_252",
	"unused_253",
	"unused_254",
	"unused_255",
	"BTN_0",
	"BTN_1",
	"BTN_2",
	"BTN_3",
	"BTN_4",
	"BTN_5",
	"BTN_6",
	"BTN_7",
	"BTN_8",
	"BTN_9",
	"unused_266",
	"unused_267",
	"unused_268",
	"unused_269",
	"unused_270",
	"unused_271",
	"BTN_LEFT",
	"BTN_RIGHT",
	"BTN_MIDDLE",
	"BTN_SIDE",
	"BTN_EXTRA",
	"BTN_FORWARD",
	"BTN_BACK",
	"BTN_TASK",
	"unused_280",
	"unused_281",
	"unused_282",
	"unused_283",
	"unused_284",
	"unused_285",
	"unused_286",
	"unused_287",
	"BTN_TRIGGER",
	"BTN_THUMB",
	"BTN_THUMB2",
	"BTN_TOP",
	"BTN_TOP2",
	"BTN_PINKIE",
	"BTN_BASE",
	"BTN_BASE2",
	"BTN_BASE3",
	"BTN_BASE4",
	"BTN_BASE5",
	"BTN_BASE6",
	"unused_301",
	"unused_302",
	"unused_303",
	"BTN_DEAD",
	"BTN_A",
	"BTN_B",
	"BTN_C",
	"BTN_X",
	"BTN_Y",
	"BTN_Z",
	"BTN_TL",
	"BTN_TR",
	"BTN_TL2",
	"BTN_TR2",
	"BTN_SELECT",
	"BTN_START",
	"BTN_MODE",
	"BTN_THUMBL",
	"BTN_THUMBR",
	"unused_319",
	"BTN_TOOL_PEN",
	"BTN_TOOL_RUBBER",
	"BTN_TOOL_BRUSH",
	"BTN_TOOL_PENCIL",
	"BTN_TOOL_AIRBRUSH",
	"BTN_TOOL_FINGER",
	"BTN_TOOL_MOUSE",
	"BTN_TOOL_LENS",
	"unused_328",
	"unused_329",
	"BTN_TOUCH",
	"BTN_STYLUS",
	"BTN_STYLUS2",
	"BTN_TOOL_DOUBLETAP",
	"BTN_TOOL_TRIPLETAP",
	"unused_335",
	"BTN_GEAR_DOWN",
	"BTN_GEAR_UP",
	"unused_338",
	"unused_339",
	"unused_340",
	"unused_341",
	"unused_342",
	"unused_343",
	"unused_344",
	"unused_345",
	"unused_346",
	"unused_347",
	"unused_348",
	"unused_349",
	"unused_350",
	"unused_351",
	"KEY_OK",
	"KEY_SELECT",
	"KEY_GOTO",
	"KEY_CLEAR",
	"KEY_POWER2",
	"KEY_OPTION",
	"KEY_INFO",
	"KEY_TIME",
	"KEY_VENDOR",
	"KEY_ARCHIVE",
	"KEY_PROGRAM",
	"KEY_CHANNEL",
	"KEY_FAVORITES",
	"KEY_EPG",
	"KEY_PVR",
	"KEY_MHP",
	"KEY_LANGUAGE",
	"KEY_TITLE",
	"KEY_SUBTITLE",
	"KEY_ANGLE",
	"KEY_ZOOM",
	"KEY_MODE",
	"KEY_KEYBOARD",
	"KEY_SCREEN",
	"KEY_PC",
	"KEY_TV",
	"KEY_TV2",
	"KEY_VCR",
	"KEY_VCR2",
	"KEY_SAT",
	"KEY_SAT2",
	"KEY_CD",
	"KEY_TAPE",
	"KEY_RADIO",
	"KEY_TUNER",
	"KEY_PLAYER",
	"KEY_TEXT",
	"KEY_DVD",
	"KEY_AUX",
	"KEY_MP3",
	"KEY_AUDIO",
	"KEY_VIDEO",
	"KEY_DIRECTORY",
	"KEY_LIST",
	"KEY_MEMO",
	"KEY_CALENDAR",
	"KEY_RED",
	"KEY_GREEN",
	"KEY_YELLOW",
	"KEY_BLUE",
	"KEY_CHANNELUP",
	"KEY_CHANNELDOWN",
	"KEY_FIRST",
	"KEY_LAST",
	"KEY_AB",
	"KEY_NEXT",
	"KEY_RESTART",
	"KEY_SLOW",
	"KEY_SHUFFLE",
	"KEY_BREAK",
	"KEY_PREVIOUS",
	"KEY_DIGITS",
	"KEY_TEEN",
	"KEY_TWEN",
	"KEY_VIDEOPHONE",
	"KEY_GAMES",
	"KEY_ZOOMIN",
	"KEY_ZOOMOUT",
	"KEY_ZOOMRESET",
	"KEY_WORDPROCESSOR",
	"KEY_EDITOR",
	"KEY_SPREADSHEET",
	"KEY_GRAPHICSEDITOR",
	"KEY_PRESENTATION",
	"KEY_DATABASE",
	"KEY_NEWS",
	"KEY_VOICEMAIL",
	"KEY_ADDRESSBOOK",
	"KEY_MESSENGER",
	"unused_431",
	"unused_432",
	"unused_433",
	"unused_434",
	"unused_435",
	"unused_436",
	"unused_437",
	"unused_438",
	"unused_439",
	"unused_440",
	"unused_441",
	"unused_442",
	"unused_443",
	"unused_444",
	"unused_445",
	"unused_446",
	"unused_447",
	"KEY_DEL_EOL",
	"KEY_DEL_EOS",
	"KEY_INS_LINE",
	"KEY_DEL_LINE",
	"unused_452",
	"unused_453",
	"unused_454",
	"unused_455",
	"unused_456",
	"unused_457",
	"unused_458",
	"unused_459",
	"unused_460",
	"unused_461",
	"unused_462",
	"unused_463",
	"KEY_FN",
	"KEY_FN_ESC",
	"KEY_FN_F1",
	"KEY_FN_F2",
	"KEY_FN_F3",
	"KEY_FN_F4",
	"KEY_FN_F5",
	"KEY_FN_F6",
	"KEY_FN_F7",
	"KEY_FN_F8",
	"KEY_FN_F9",
	"KEY_FN_F10",
	"KEY_FN_F11",
	"KEY_FN_F12",
	"KEY_FN_1",
	"KEY_FN_2",
	"KEY_FN_D",
	"KEY_FN_E",
	"KEY_FN_F",
	"KEY_FN_S",
	"KEY_FN_B",
	"unused_485",
	"unused_486",
	"unused_487",
	"unused_488",
	"unused_489",
	"unused_490",
	"unused_491",
	"unused_492",
	"unused_493",
	"unused_494",
	"unused_495",
	"unused_496",
	"KEY_BRL_DOT1",
	"KEY_BRL_DOT2",
	"KEY_BRL_DOT3",
	"KEY_BRL_DOT4",
	"KEY_BRL_DOT5",
	"KEY_BRL_DOT6",
	"KEY_BRL_DOT7",
	"KEY_BRL_DOT8",
	"unused_505",
	"unused_506",
	"unused_507",
	"unused_508",
	"unused_509",
	"KEY_UNKNOWN",
	"KEY_MAX",
};