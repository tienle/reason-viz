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
    ~shape="circle",
    ~props=
      ShapeValue.[
        ("x", Float(100.0)),
        ("y", Float(100.0)),
        ("size", PairInt(40, 40)),
      ],
    ~styles=Style.[stroke("black")],
    ~label=Node.Label.make(~text="circle", ~position=`bottom, ()),
    (),
  ),
  Node.Model.make(
    ~id="node2",
    ~shape="rect",
    ~props=
      ShapeValue.[
        ("x", Float(300.0)),
        ("y", Float(200.0)),
        ("size", PairInt(40, 40)),
      ],
    ~styles=Style.[stroke("black")],
    ~label=Node.Label.make(~text="center", ~position=`bottom, ()),
    (),
  ),
  Node.Model.make(
    ~id="node3",
    ~shape="rect",
    ~props=
      ShapeValue.[
        ("x", Float(400.0)),
        ("y", Float(100.0)),
        ("size", PairInt(40, 40)),
      ],
    ~styles=Style.[stroke("black")],
    ~label=Node.Label.make(~text="circle 7", ~position=`bottom, ()),
    (),
  ),
  Node.Model.make(
    ~id="node4",
    ~shape="rect",
    ~props=
      ShapeValue.[
        ("x", Float(300.0)),
        ("y", Float(400.0)),
        ("size", PairInt(40, 40)),
      ],
    ~styles=Style.[stroke("black")],
    ~label=Node.Label.make(~text="rect", ~position=`bottom, ()),
    (),
  ),
];
let edges = [
  Edge.Model.make(
    ~id="edge-cat",
    ~source=`NodeId("node1"),
    ~target=`NodeId("node2"),
    ~shape="circle-running",
    (),
  ),
  Edge.Model.make(
    ~source=`NodeId("node3"),
    ~target=`NodeId("node2"),
    ~shape="line-dash",
    (),
  ),
  Edge.Model.make(
    ~source=`NodeId("node2"),
    ~target=`NodeId("node4"),
    ~shape="line-growth",
    (),
  ),
];

module CircleRunningEdge =
  ReasonViz.EdgeShape.Make({
    include ReasonViz.EdgeShape.Cubic;
  });

module LineDashEdge =
  ReasonViz.EdgeShape.Make({
    include ReasonViz.EdgeShape.Cubic;
    let lineDash = [|4, 2, 1, 2|];
    let interval = 9;
    let dashArray = [|
      [|0, 1|],
      [|0, 2|],
      [|1, 2|],
      [|0, 1, 1, 2|],
      [|0, 2, 1, 2|],
      [|1, 2, 1, 2|],
      [|2, 2, 1, 2|],
      [|3, 2, 1, 2|],
      [|4, 2, 1, 2|],
    |];

    let afterDraw = (edge: ReasonViz.Edge.t) => {
      let shape = edge.shape;
      let length = Canvas.Shape.getTotalLength(shape);
      let totalDash = ref([||]);

      let size = int_of_float(length) / interval;
      for (_i in 0 to size) {
        totalDash := Array.append(totalDash^, lineDash);
      };
      Js.log(totalDash);
      let index = ref(0);

      Canvas.Shape.animate(
        shape,
        ~attrs={
          "onFrame": _ratio => {
            index :=
              Int32.rem(Int32.of_int(index^ + 1), Int32.of_int(interval))
              |> Int32.to_int;

            {"lineDash": Belt.Array.concat(dashArray[index^], totalDash^)};
          },
          "repeat": true,
        },
        ~duration=2000,
        (),
      );

      ();
    };
  });

module LineGrowthEdge =
  ReasonViz.EdgeShape.Make({
    include ReasonViz.EdgeShape.Cubic;

    let afterDraw = (edge: ReasonViz.Edge.t) => {
      let shape = edge.shape;
      let length = Canvas.Shape.getTotalLength(shape);

      Canvas.Shape.animate(
        shape,
        ~attrs={
          "onFrame": ratio => {
            let l = ratio *. length;
            {"lineDash": [|l, length -. l|]};
          },
          "repeat": true,
        },
        ~duration=2000,
        (),
      );

      ();
    };
  });

ReasonViz.EdgeShape.register("circle-running", (module CircleRunningEdge));
ReasonViz.EdgeShape.register("line-dash", (module LineDashEdge));
ReasonViz.EdgeShape.register("line-growth", (module LineGrowthEdge));

Event.subscribe(
  g.events.onNodeMouseEnter,
  ((_, n)) => {
    Canvas.Shape.attr(n.shape, "fill", "pink");
    Graph.setNodeState(n, ~key="animate", ~value="running");
  },
);

Event.subscribe(
  g.events.onNodeMouseLeave,
  ((_, n)) => {
    Canvas.Shape.attr(n.shape, "fill", "white");
    Graph.setNodeState(n, ~key="animate", ~value="");
  },
);

let nyanCatsCount = ref(0);

Event.subscribe(
  g.events.onNodeClick,
  ((_, node)) => {
    nyanCatsCount := nyanCatsCount^ + 1;
    let fillColor = Canvas.Shape.attr(node.shape, "fill");
    fillColor("indigo");
    Js.Global.setTimeout(() => fillColor("pink"), 200);

    node.edges
    |> List.filter(e => e.GraphTypes.model.id == "edge-cat")
    |> List.map((edge: GraphTypes.edge) => {
         let startPoint = Canvas.Shape.getPoint(edge.shape, ~ratio=0.0);
         Canvas.Group.addShape(
           edge.group,
           "image",
           {
             "id": "cat" ++ string_of_int(nyanCatsCount^),
             "className": "nyan",
             "attrs": {
               "img": "http://www.nyan.cat/cats/original.gif",
               "width": 30,
               "x": startPoint.x,
               "y": startPoint.y,
             },
           },
         );
       });
    Graph.setNodeState(node, ~key="animate", ~value="running");
    ();
  },
);

let animateCircleEdge = (edge: ReasonViz.Edge.t) => {
  let shape = edge.shape;
  let cats =
    Canvas.Group.findAllShapes(edge.group, s =>
      Canvas.Shape.get(s, "className") == "nyan"
    );

  cats->Belt.Array.map(cat =>
    Canvas.Shape.animate(
      cat,
      ~attrs={
        "onFrame": ratio => {
          let point = Canvas.Shape.getPoint(shape, ~ratio);
          point |> pointToJs;
        },
        "repeat": true,
      },
      ~duration=3000,
      (),
    )
  );

  ();
};

let stopAnimate = (edge: GraphTypes.edge) => {
  Canvas.Group.findAllShapes(edge.group, s =>
    Canvas.Shape.get(s, "className") == "nyan"
  )
  |> Array.iter(shape => {
       Canvas.Shape.stopAnimate(shape);
       Canvas.Shape.remove(shape);
     });
};

Event.subscribe(
  g.events.onNodeStateUpdated,
  ((node, key, value, _)) => {
    let animateEdges = edges => {
      switch (key, value) {
      | ("animate", "running") => edges |> List.iter(animateCircleEdge)
      | ("animate", _) => edges |> List.iter(stopAnimate)
      | (_, _) => ()
      };
    };

    node.edges
    |> List.filter(e => e.GraphTypes.model.id == "edge-cat")
    |> animateEdges;
  },
);

nodes |> Graph.addNodes(g);
edges |> Graph.addEdges(g);
Graph.render(g);
