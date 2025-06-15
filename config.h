/* See LICENSE file for copyright and license details. */

static const char term              = "st";
static const char browser           = "firefox";
/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const int startwithgaps	    = 1;	 /* 1 means gaps are used by default */
static const unsigned int gappx     = 10;       /* default gap between windows in pixels */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int horizpadbar        = 2;        /* horizontal padding for statusbar */
static const int vertpadbar         = 0;        /* vertical padding for statusbar */
static const char *fonts[]          = { "Symbols Nerd Font Mono:size=11:style=Regular", "Roboto Mono:size=11:style=Italic"};
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {term, "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {term, "-n", "spfm", "-g", "144x41", "-e", "ranger", NULL };
const char *spcmd3[] = {"keepassxc", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spranger",    spcmd2},
	{"keepassxc",   spcmd3},
};

/* tagging */
static const char *tags[] = { "󰽧", "󰽡", "󰽨", "", "󰽦", "󰽣", "󰽥"};
static const char *tagsalt[] = { "a", "s", "d", "f", "w", "e", "g"};
static const char *tagsnum[] = { "1", "2", "3", "4", "5", "6", "7"};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class        instance    title       tags mask     switchtotag    isfloating   monitor */
	{ "Gimp",	      NULL,			  NULL,		    0,            0,				     1,			      -1 },
	{ browser,      NULL,			  NULL,		    1 << 3,			  1,             0,			      -1 },
	//{ NULL,		  "spterm",		NULL,		SPTAG(0),		1,			 -1 },
	//{ NULL,		  "spfm",		NULL,		SPTAG(1),		1,			 -1 },
	//{ NULL,		  "keepassxc",	NULL,		SPTAG(2),		0,			 -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static int attachbelow = 1;    /* 1 means attach after the currently active window */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* helper for launching gtk application */
#define GTKCMD(cmd) { .v = (const char*[]){ "/usr/bin/gtk-launch", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { term, NULL };
static const char *browsercmd[] = { "firefox", NULL};
static const char *pbrowsercmd[] = { "firefox", "-private-window", NULL};
static const char *btcmd[] = { "dmenu-bluetooth", NULL};
static const char *mailcmd[] = { "thunderbird", NULL};
static const char *scrotcmd[] = { "flameshot", "gui", NULL};
static const char *powercmd[] = { "powermenu.sh", NULL};

static const char *mpdtogglecmd[] = { "mpc", "toggle", NULL};
static const char *mpdnextcmd[] = { "mpc", "next", NULL};
static const char *mpdprevcmd[] = { "mpc", "prev", NULL};
static const char *volraisecmd[] = { "pamixer", "-i", "10", NULL};
static const char *vollowercmd[] = { "pamixer", "-d", "10", NULL};
static const char *volmutecmd[] = { "pamixer", "--toggle-mute", NULL};
static const char *audioswtchcmd[] = { "audioswitch.sh", NULL};

static const Arg tagexec[] = {
	{ .v = NULL },
	{ .v = termcmd },
	{ .v = termcmd },
	{ .v = browsercmd },
	{ .v = termcmd },
	{ .v = termcmd },
	{ .v = termcmd },
	{ .v = termcmd },
	{ .v = termcmd }
};

static const Key keys[] = {
  /* ### LAUNCH PROGRAMS ### */
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_u,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      spawn,          {.v = browsercmd } },
  /* ### MEDIA KEYS ### */
	/* modifier                     key        function        argument */
//  { 0,                            XF86XK_AudioPlay,             spawn,          {.v = mpdtogglecmd } },
//	{ 0,                            XF86XK_AudioNext,             spawn,          {.v = mpdnextcmd } },
//	{ 0,                            XF86XK_AudioPrev,             spawn,          {.v = mpdprevcmd } },
//	{ 0,                            XF86XK_AudioRaiseVolume,      spawn,          {.v = volraisecmd } },
//	{ 0,                            XF86XK_AudioLowerVolume,      spawn,          {.v = vollowercmd } },
//	{ 0,                            XF86XK_AudioMute,             spawn,          {.v = volmutecmd } },
  /* ### WM Keys ### */
	/* modifier                     key        function        argument */
	//{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_equal,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_minus,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Tab,    zoom,           {0} },
	{ MODKEY|ShiftMask,             XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_r,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,            			      XK_y,  	   togglescratch,  {.ui = 0 } },
	{ MODKEY,            			      XK_p,	     togglescratch,  {.ui = 1 } },
	{ MODKEY,            			      XK_o,	     togglescratch,  {.ui = 2 } },
//	{ MODKEY,                       XK_lefbracket,  setgaps,        {.i = -5 } },
//	{ MODKEY,                       XK_rightbracket,  setgaps,        {.i = +5 } },
//	{ MODKEY|ShiftMask,             XK_leftbracket,  setgaps,        {.i = GAP_RESET } },
//	{ MODKEY|ShiftMask,             XK_rightbracket,  setgaps,        {.i = GAP_TOGGLE} },
	TAGKEYS(                        XK_a,                      0)
	TAGKEYS(                        XK_s,                      1)
	TAGKEYS(                        XK_d,                      2)
	TAGKEYS(                        XK_f,                      3)
	TAGKEYS(                        XK_w,                      4)
	TAGKEYS(                        XK_e,                      5)
	TAGKEYS(                        XK_g,                      6)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button1,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button2,        spawntag,       {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

