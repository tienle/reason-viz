type declaration = [ | `declaration(string, string)];

type rule = [ declaration];

let d = (property, value) => `declaration((property, value));

let make = styles => {
  let toKeyValuePair = rule =>
    switch (rule) {
    | `declaration(k, v) => (k, v)
    };
  styles |> List.map(toKeyValuePair) |> Js.Dict.fromList;
};

let cursor = x =>
  d(
    "cursor",
    switch (x) {
    | `pointer => "pointer"
    | `alias => "alias"
    | `allScroll => "all-scroll"
    | `auto => "auto"
    | `cell => "cell"
    | `contextMenu => "context-menu"
    | `default => "default"
    | `none => "none"
    | `crosshair => "crosshair"
    | `copy => "copy"
    | `grab => "grab"
    | `grabbing => "grabbing"
    | `help => "help"
    | `move => "move"
    | `notAllowed => "not-allowed"
    | `progress => "progress"
    | `text => "text"
    | `wait => "wait"
    | `zoomIn => "zoom-in"
    | `zoomOut => "zoom-out"
    },
  );

let lineCap = x => {
  d(
    "lineCap",
    switch (x) {
    | `butt => "butt"
    | `round => "round"
    | `square => "square"
    },
  );
};

let lineJoin = x => {
  d(
    "lineJoin",
    switch (x) {
    | `bevel => "bevel"
    | `round => "round"
    | `miter => "miter"
    },
  );
};

let lineWidth = x => {
  d("lineWidth", string_of_int(x));
};

let miterLimit = x => {
  d("miterLimit", string_of_int(x));
};

let lineDash = x => {
  d(
    "miterLimit",
    Js.Json.stringifyAny(x)->(Belt.Option.getWithDefault("")),
  );
};

let textAlign = x => {
  d(
    "textAlign",
    switch (x) {
    | `center => "center"
    | `end_ => "end"
    | `left => "left"
    | `right => "right"
    | `start => "start"
    },
  );
};

let textBaseline = x => {
  d(
    "textBaseline",
    switch (x) {
    | `top => "top"
    | `middle => "middle"
    | `bottom => "bottom"
    | `alphabetic => "alphabetic"
    | `hanging => "hanging"
    },
  );
};

let fontStyle = x => {
  d(
    "fontStyle",
    switch (x) {
    | `italic => "italic"
    | `normal => "normal"
    | `oblique => "oblique"
    },
  );
};

let fontVariant = x => {
  d(
    "fontVariant",
    switch (x) {
    | `normal => "normal"
    | `smallCaps => "smallCaps"
    },
  );
};

let fontWeight = x => {
  d("fontWeight", string_of_int(x));
};

let fontSize = x => {
  d("fontWeight", x);
};

let fontFamily = x => {
  d("fontWeight", x);
};

let fill = x => {
  d("fill", x);
};

let stroke = x => {
  d("stroke", x);
};

let shadowColor = x => {
  d("shadowColor", x);
};

let shadowBlur = x => {
  d("shadowBlur", x);
};

let shadowOffsetX = x => {
  d("shadowOffsetX", x |> string_of_int);
};

let shadowOffsetY = x => {
  d("shadowOffsetY", x |> string_of_int);
};

let opacity = x => {
  d("opacity", x |> Js.Float.toString);
};

let globalCompositeOperation = x => {
  d("globalCompositeOperation", x);
};
