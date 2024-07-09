import nipplejs from "nipplejs";
import "sanitize.css";
import "./style.css";

const elms = {
  main: {
    joystickZone: document.querySelector("#joystick-zone"),
    buttonsZone: document.querySelector("#buttons-zone"),
  },
  settings: {
    host: document.querySelector("#settings-host"),
    interval: document.querySelector("#settings-interval"),
    buttons: document.querySelector("#settings-buttons"),
  },
};

let host = "http://localhost";
let interval = 200;
let showButtons = true;

elms.settings.host.oninput = ({ target: input }) => {
  console.log(`Modify host ${host} -> ${input.value}`);
  host = input.value;
};

elms.settings.interval.oninput = ({ target: input }) => {
  console.log(`Modify interval ${interval} -> ${input.value}`);
  interval = input.value;
};

elms.settings.buttons.oninput = ({ target: checkbox }) => {
  console.log(`Modify showButtons ${showButtons} -> ${checkbox.checked}`);
  showButtons = checkbox.checked;
  if (showButtons) elms.main.buttonsZone.classList.remove("erase");
  else elms.main.buttonsZone.classList.add("erase");
};

const joystickManager = nipplejs.create({
  zone: elms.main.joystickZone,
  color: "#55CCFF",
  size: 160,
  mode: "dynamic",
});

document.addEventListener("DOMContentLoaded", ({}) => {
  for (const input of Object.values(elms.settings)) {
    input.oninput?.({ target: input });
  }
});
