[@react.component]
let make = (~t: string, ~w: int, ~h: int, ~s: option(string), ~onClick) => {
  open Css;
  let defaultButtonStyle =
    style([fontSize(px(w)), width(px(w)), height(px(h))]);

  let buttonStyle =
    switch (s) {
    | None => defaultButtonStyle
    | Some(s) => merge([s, defaultButtonStyle])
    };
  let viewBox = "0 0 " ++ string_of_int(w) ++ " " ++ string_of_int(h);
  let fontSize = string_of_int(w / 2) ++ "px";
  <button className=buttonStyle onClick>
    <svg viewBox>
      <text fontSize x="0" y={string_of_int(w / 2)} dy="1em" dx=".25em">
        {ReasonReact.string(t)}
      </text>
    </svg>
  </button>;
};