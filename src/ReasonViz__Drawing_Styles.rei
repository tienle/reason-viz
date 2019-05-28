type rule;

let make: list(rule) => Js.Dict.t(string);

let cursor:
  [
    | `pointer
    | `alias
    | `allScroll
    | `auto
    | `cell
    | `contextMenu
    | `default
    | `none
    | `crosshair
    | `copy
    | `grab
    | `grabbing
    | `help
    | `move
    | `notAllowed
    | `progress
    | `text
    | `wait
    | `zoomIn
    | `zoomOut
  ] =>
  rule;

let lineCap: [< | `butt | `round | `square ] => rule;
let lineJoin: [< | `bevel | `round | `miter ] => rule;
let lineWidth: int => rule;
let miterLimit: int => rule;
let lineDash: array(int) => rule;

/* let symbol: string => rule; */
/* let clip: 'a => rule; */

let textAlign: [< `center | `end_ | `left | `right | `start ] => rule;
let textBaseline: [< `top | `middle | `bottom | `alphabetic | `hanging ] => rule;

let fontStyle: [< `italic | `normal | `oblique ] => rule;

let fontVariant: [< | `normal | `smallCaps] => rule;
let fontWeight: int => rule;
let fontSize: string => rule;
let fontFamily: string => rule;

/* TODO: Strong typed for these attributes */
let fill: string => rule;
let stroke: string => rule;
let shadowColor: string => rule;
let shadowBlur: string => rule;
let shadowOffsetX: int => rule;
let shadowOffsetY: int => rule;
let opacity: float => rule;
let globalCompositeOperation: string => rule;
