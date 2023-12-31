
## blink1-tiny-server

A simple HTTP to blink(1) gateway

- Uses blink1-lib to talk to blink(1)
- Uses Mongoose : https://github.com/cesanta/mongoose which is included

Supports several of the URIs of the Blink1Control API server,
but implemented as a tiny portable C webserver.

To build:
```
cd blink1-tool
make blink1-tiny-server
```
Use:
```
./blink1-tiny-server -p 8000
```
By default the HTML API examples are built-in and served. To try them out,
open a browser to: http://localhost:8000/

To disable the HTML examples, use:
```
./blink1-tiny-server -p 8000 -no-html
```

Usage:
```
% ./blink1-tiny-server --help
Usage:
  blink1-tiny-server [options]
where [options] can be:
  --port port, -p port           port to listen on (default 8000)
  --baseurl url, -U url          set baseurl to listen in (default http://localhost:8000)
  --no-html                      do not serve static HTML help
  --version                      version of this program
  --help, -h                     this help page

Supported URIs:
  /blink1/ -- simple status page
  /blink1/id -- get blink1 serial number
  /blink1/on -- turn blink(1) full bright white
  /blink1/off -- turn blink(1) dark
  /blink1/red -- turn blink(1) solid red
  /blink1/green -- turn blink(1) solid green
  /blink1/blue -- turn blink(1) solid blue
  /blink1/cyan -- turn blink(1) solid cyan
  /blink1/yellow -- turn blink(1) solid yellow
  /blink1/magenta -- turn blink(1) solid magenta
  /blink1/fadeToRGB -- turn blink(1) specified RGB color by 'rgb' arg
  /blink1/blink -- blink the blink(1) the specified RGB color
  /blink1/pattern/play -- play color pattern specified by 'pattern' arg
  /blink1/random -- turn the blink(1) a random color

Supported query arguments: (not all urls support all args)
  'rgb'    -- hex RGB color code. e.g. 'rgb=FF9900' or 'rgb=%23FF9900
  'time'   -- time in seconds. e.g. 'time=0.5'
  'bright' -- brightness, 1-255, 0=full e.g. half-bright 'bright=127'
  'ledn'   -- which LED to set. 0=all/1=top/2=bot, e.g. 'ledn=0'
  'millis' -- milliseconds to fade, or blink, e.g. 'millis=500'
  'count'  -- number of times to blink, for /blink1/blink, e.g. 'count=3'
  'pattern'-- color pattern string (e.g. '3,00ffff,0.2,0,000000,0.2,0')

Examples:
  /blink1/blue?bright=127 -- set blink1 blue, at half-intensity
  /blink1/fadeToRGB?rgb=FF00FF&millis=500 -- fade to purple over 500ms
  /blink1/pattern/play?pattern=3,00ffff,0.2,0,000000,0.2,0 -- blink cyan 3 times

```
