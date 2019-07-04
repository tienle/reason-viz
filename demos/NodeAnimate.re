open ReasonViz__Types;

module Canvas = ReasonViz.Canvas;
module Node = ReasonViz.Node;
module Edge = ReasonViz.Edge;
module Style = ReasonViz.Style;
module Graph = ReasonViz.Graph;
module Event = ReasonViz.Event;
module GraphTypes = ReasonViz.GraphTypes;
module ShapeValue = GraphTypes.ShapeValue;

let graphOptions =
  ReasonViz.GraphOptions.create(
    ~containerId="index",
    ~width=500,
    ~height=500,
    ~renderer="svg",
    (),
  );

let g: Graph.t = Graph.create(~graphOptions);

let nodes = [
  Node.Model.make(
    ~id="node1",
    ~shape="circle-animate",
    ~props=
      ShapeValue.[
        ("x", Float(100.0)),
        ("y", Float(100.0)),
        ("size", PairInt(20, 20)),
      ],
    ~styles=Style.[stroke("black")],
    ~label=Node.Label.make(~text="circle", ~position=`bottom, ()),
    (),
  ),
  Node.Model.make(
    ~id="node2",
    ~shape="background-animate",
    ~props=
      ShapeValue.[
        ("x", Float(300.0)),
        ("y", Float(200.0)),
        ("size", PairInt(20, 20)),
      ],
    ~styles=Style.[stroke("black")],
    (),
  ),
  Node.Model.make(
    ~id="node3",
    ~shape="inner-animate",
    ~props=
      ShapeValue.[
        ("x", Float(400.0)),
        ("y", Float(100.0)),
        (
          "img",
          Str(
            "data:image/webp;base64,UklGRq4FAABXRUJQVlA4IKIFAABwHwCdASo8ADwAPiEMhEGhhv6rQAYAgS2NHsdCq/4D8AOoA64OEUAj/XPxVwyvRGvyO/gGxN/t3oK/1X6zesX3L/p/RP/2HCgKAB9AGeAbCB+AGwAbQBtA/8c/m/4PYHTonm+SzRH6B9sv2i/rOZC+G/ln9l/ML/GdoD7APcA/TD+09QDzAfrX+vHYM9AD+Uf0zrAPQA/bH0vv2t+CH9qf2R+A39e6X098/I7IAcLcs8Gjmc/9T7gPbX9H+wJ+rPVV9En9ZmBI5oUiYhkYHIVjRr9hzCTPcV5Rs/wjjIHkxPgtr/3ALZSuUm146HHwqQVA23hnnqH/4aJ/k4v4hU6RBZ0AAAD+//8ARPyL9yWIlAbWBAD0oKSqlYWreuRa3Oj02u+TvSQS8iwMYewUYTWLDNp9wOlFJaWnqE+za35UwUXuDAT6T0I4fwY+u+qrRVhl+S1ir4X7BQiNswug5AX+MjQcXEeUwfSIEUT+DFPCr+BUiwTbFxLni7fv61vRbmXoauLz4tiqOFTzEGP8tNXP5+H7mZVGfNjIxapT3FGUtqBdp/SD5cTOYOkn2fawkpqpCSqf2+CfiGWtIF673fEzlk/hIbWDhQ81C/ddxLn609d/5efckbdZ8HZbhhVmM82/Uat7CFmw1SH5xCxRxEEhjpf1EP1Xn5q9VZfm1+OFTab/MN67Xha8K//5oVBlMgZALE653X0fas/+2xMqiyCu5Wa7PHsCwbBwqROfNmzi4LPOTjkFPHVKDD1Nfj4/sul6cANdF68rf2jszlyZsUUoLTP7H3swSroc3ssNXSRVAcYd7+iBZpfoAYWvKgnr+Hv62fHZX5ZbjYbYzVjq6fsXkubto858NuUx4+ILb5y7dP6W3/IYVeUSF0yZseKIZhOMs9BBf5uB2Y3Ott//+1OG7hYINzcqigrzWAOJbSmVw3G0ULywkobx+rvfk8VmZFzQGgP/+4T74mp/vsZyM1NLguiTO2gNO05tcpXwveq5mrcweXrJ/bRZDmU2KBrnXhkXq+735c+UHTFq4h4jMOPm5shKioB6XaqhUf3DJlMg8937g/SKjrgD0H5sNm0/k4FfilBbcrsjc3dd6cwEYJo3CNhe3SpNJ2geNXyV4/hq/BZXZ1kiVknjmf5cUx7Tv/9Sb+fQ/DgAsRNSz1wiVodiLjP7aVrkxbWx5gJ8U/j0o1Ipm/nyDZRPrQXmbPAcy1eDDejxTDBKe42ElHpC2QlFdhOedsp4i9QVjt8EqWGy1YzPaGqZhCVg/LWt8/+4BmiCzNGtpR21MGJf4kI/n/1sbe36e1QBCBAx4EVfTM82ZM2lh0P189e7eY0A3NzXWVrUek8SEn+DYYCQeEaC5hDHGreFHT1baY6KyrFx3G9oMm3fLrCqmNjFRnZa3LB/5m8FgCpq9B/1OCLRE5GzVTZnVzj/4V38PgCIpX16Kznijf01+MkDeS9oCF2hEXQ9tr+mPLrjGy4Cg5fyLgyCj1fUq33nMf79Svli2h83m3gqkoxJcXvBetFQP8V/gRjBNGmFXK5TfwLhbolWEjDqUGK3n+hxzQLif9zreYO88EIRTUNbzE1/Sn7rBEtjB0uawNje5OubWsB62SOlMZoZpxrDbMb4UvQrODPhSafmhcYe9zm/dHxssMfUthhDKjyMhoRhngPjbzfGXmIV2Omgrn/zbefK/PawUGSH6x4Qk4HCN4/X8S+XCf51JJtOQeHST/yfwg69uMkE07SONnhGUrL6j5oQn6JI+zkaH/H/P/Ti/pfOTfAWxQNiMvWX08mqbuUweFSQ/G5YUP/uCvZAXutf1+Nhl2jj/n4/fPOihPjwvfFnnjOaQvs9PSpF33d+396LASZ3IID/4UP4pf9eOMXw82ccoUUUHX6MfBWyBDvARCrdPmerUwKwW+lBIAe1dsAAAA==",
          ),
        ),
        ("size", PairInt(40, 40)),
      ],
    ~styles=Style.[stroke("black")],
    (),
  ),
  Node.Model.make(
    ~id="node4",
    ~shape="circle",
    ~props=
      ShapeValue.[
        ("x", Float(300.0)),
        ("y", Float(400.0)),
        ("size", PairInt(40, 40)),
      ],
    ~styles=Style.[stroke("black")],
    (),
  ),
];
let edges = [
  Edge.Model.make(
    ~source=`NodeId("node1"),
    ~target=`NodeId("node2"),
    ~shape="line",
    (),
  ),
  Edge.Model.make(
    ~source=`NodeId("node3"),
    ~target=`NodeId("node2"),
    ~shape="line",
    (),
  ),
  Edge.Model.make(
    ~source=`NodeId("node2"),
    ~target=`NodeId("node4"),
    ~shape="line",
    (),
  ),
];

module CircleAnimate =
  ReasonViz.NodeShape.Make({
    include ReasonViz.NodeShape.Circle;

    let afterDraw = (node: ReasonViz.Node.t) => {
      let shape = node.shape;
      Canvas.Shape.animate(
        shape,
        ~attrs={
          "repeat": true,
          "onFrame": ratio => {
            let diff = ratio <= 0.5 ? ratio *. 10.0 : (1.0 -. ratio) *. 10.0;
            {"r": float_of_int(node.model.size.width / 2) +. diff};
          },
        },
        ~duration=3000,
        ~easing="easeCubic",
        (),
      );
    };
  });

module BackgroundAnimate =
  ReasonViz.NodeShape.Make({
    include ReasonViz.NodeShape.Circle;
    let afterDraw = (node: ReasonViz.Node.t) => {
      let r = node.model.size.width / 2;

      let backs =
        List.map(
          index =>
            Canvas.Group.addShape(
              node.group,
              "circle",
              {
                "zIndex": index,
                "attrs": {
                  "x": 0,
                  "y": 0,
                  "r": r,
                  "fill": "#333",
                  "opacity": 0.6,
                },
              },
            ),
          [(-3), (-2), (-1)],
        );

      Canvas.Group.sort(node.group);

      List.iteri(
        (i, shape) =>
          Canvas.Shape.animate(
            shape,
            ~attrs={"repeat": true, "opacity": 0.1, "r": r + 10},
            ~duration=3000,
            ~easing="easeCubic",
            ~delay=i * 1000,
            (),
          ),
        backs,
      );

      ();
    };
  });

module InnerAnimate =
  ReasonViz.NodeShape.Make({
    include ReasonViz.NodeShape.Circle;

    let afterDraw = (node: ReasonViz.Node.t) => {
      let {width, height} = node.model.size;
      let w = width - 12;
      let h = height - 12;
      let img =
        node.model.props.getExn("img") |> GraphTypes.ShapeValue.toString;

      let image =
        Canvas.Group.addShape(
          node.group,
          "image",
          {
            "attrs": {
              "x": - w / 2,
              "y": - h / 2,
              "width": w,
              "height": h,
              "img": img,
            },
          },
        );

      Canvas.Shape.animate(
        image,
        ~attrs={
          "repeat": true,
          "onFrame": ratio => {
            open ReasonViz.Util;
            let matrix = createMat3();
            let toMatrix =
              transformMatrix(matrix, [|("r", ratio *. Math.pi *. 2.0)|]);
            {"matrix": toMatrix};
          },
        },
        ~duration=3000,
        ~easing="easeCubic",
        (),
      );
      ();
    };
  });

ReasonViz.NodeShape.register(
  "background-animate",
  (module BackgroundAnimate),
);
ReasonViz.NodeShape.register("circle-animate", (module CircleAnimate));
ReasonViz.NodeShape.register("inner-animate", (module InnerAnimate));

Event.subscribe(
  g.events.onNodeMouseEnter,
  ((e, node)) => {
    Js.log("node:mouseenter")
    Js.log(e);
    Js.log(node);
  });

Event.subscribe(
  g.events.onMouseEnter,
  ((e, item)) => {
    Js.log("mouseenter")
    Js.log(e);
    Js.log(item);
  });


nodes |> Graph.addNodes(g);
edges |> Graph.addEdges(g);
Graph.render(g);
