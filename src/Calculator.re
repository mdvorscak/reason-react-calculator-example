[%%debugger.chrome];
module Theme = {
  open Css;
  let textColor = hex("2d3436");
};
module Styles = {
  /* Open the Css module, so we can access the style properties below without prefixing them with Css. */
  open Css;

  let root =
    style([
      width(px(640)),
      height(px(640)),
      backgroundColor(hex("dfe6e9")),
      display(`flex),
      flexDirection(`column),
    ]);

  let total =
    style([
      fontSize(px(36)),
      color(Theme.textColor),
      margin(px(24)),
      padding(px(12)),
      borderRadius(px(3)),
      textAlign(`right),
      backgroundColor(white),
    ]);

  let buttonsContainer = style([display(`flex)]);

  let button =
    style([
      fontSize(px(48)),
      color(Theme.textColor),
      margin(px(36)),
      borderRadius(px(3)),
      width(px(48)),
      height(px(60)),
      cursor(`pointer),
      backgroundColor(white),
      border(px(1), `solid, hex("333333")),
    ]);

  let operatorsGroup = style([flexDirection(`column)]);

  let operatorButton =
    merge([
      button,
      style([backgroundColor(hex("74b9ff")), color(hex("FFF"))]),
    ]);

  let deleteButton =
    merge([
      button,
      style([backgroundColor(hex("d63031")), SVG.fill(hex("FFF"))]),
    ]);

  let numbersContainer = style([flexDirection(`column)]);
};

type operator =
  | Plus
  | Minus
  | Mult
  | Div
  | Nil;

/* State declaration */
type state = {
  total: float,
  operator,
};

/* Action declaration */
type action =
  | SetNumber(float)
  | Add
  | Multiply
  | Subtract
  | Divide
  | Del
  | Clear;

let pop_last_char = (str: string): string => {
  let len = String.length(str);
  String.sub(str, 0, len - 1);
};

[@react.component]
let make = () => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | SetNumber(x) =>
          switch (state.operator) {
          | Plus => {total: state.total +. x, operator: Nil}
          | Minus => {total: state.total -. x, operator: Nil}
          | Mult => {total: state.total *. x, operator: Nil}
          | Div => {total: state.total /. x, operator: Nil}
          | Nil =>
            switch (state.total) {
            | 0.0 => {...state, total: x}
            | _ => {
                ...state,
                total:
                  (Js.Float.toString(state.total) ++ Js.Float.toString(x))
                  ->float_of_string,
              }
            }
          | _ => state
          }
        | Add => {...state, operator: Plus}
        | Multiply => {...state, operator: Mult}
        | Subtract => {...state, operator: Minus}
        | Divide => {...state, operator: Div}
        | Del =>
          switch (state.total->abs_float->Js.Float.toString->String.length) {
          | 0
          | 1 => {...state, total: 0.0}
          | _ => {
              ...state,
              total:
                state.total->Js.Float.toString->pop_last_char->float_of_string,
            }
          }
        | Clear => {total: 0.0, operator: Nil}
        },
      {total: 0.0, operator: Nil},
    );

  let total = Js.Float.toString(state.total);

  <div className=Styles.root>
    <div className=Styles.total> {ReasonReact.string(total)} </div>
    <div className=Styles.buttonsContainer>
      <div className=Styles.numbersContainer>
        {ReasonReact.array(
           List.map(
             x =>
               <button
                 key={Js.Float.toString(x)}
                 className=Styles.button
                 onClick={_ => dispatch(SetNumber(x))}>
                 {ReasonReact.string(Js.Float.toString(x))}
               </button>,
             [0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0],
           )
           |> Array.of_list,
         )}
      </div>
      <div className=Styles.operatorsGroup>
        <button className=Styles.operatorButton onClick={_ => dispatch(Add)}>
          {ReasonReact.string("+")}
        </button>
        <button
          className=Styles.operatorButton onClick={_ => dispatch(Multiply)}>
          {ReasonReact.string("*")}
        </button>
        <button
          className=Styles.operatorButton onClick={_ => dispatch(Subtract)}>
          {ReasonReact.string("-")}
        </button>
        <button
          className=Styles.operatorButton onClick={_ => dispatch(Divide)}>
          {ReasonReact.string("/")}
        </button>
        <ScaleableTextButton
          w=48
          h=60
          t="Del"
          s={Some(Styles.deleteButton)}
          onClick={_ => dispatch(Del)}
        />
        <button
          className=Styles.operatorButton onClick={_ => dispatch(Clear)}>
          {ReasonReact.string("C")}
        </button>
      </div>
    </div>
  </div>;
};
