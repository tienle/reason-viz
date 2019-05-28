type labelPosition = [ | `top | `bottom | `left | `right | `center];
type point = {
  x: int,
  y: int,
};

type size = {
  width: int,
  height: int,
};

type circle = {
  mutable x: int,
  mutable y: int,
  radius: int,
};

type ellipse = {
  mutable x: int,
  mutable y: int,
  rx: int,
  ry: int,
};

type text = {content: string};

type shape('a) =
  [ | `Circle(circle) | `Ellipse(ellipse) | `Text(text)] as 'a;

module StringHash =
  Hashtbl.Make({
    type t = string;
    let equal = String.equal;
    let hash = String.length;
  });
