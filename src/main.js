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
let interval = 125;
let showButtons = true;
let intervalId = null;

async function postJoystick(angle = Math.PI / 2, force = 1) {
  const query = [
    Math.round((angle * 128) / Math.PI + (force >= 0 ? 256 : 384)) % 256,
    Math.min(Math.abs(Math.floor(force * 255)), 255),
  ]
    .map((num) => num.toString(16).toUpperCase().padStart(2, "0"))
    .join("");
  console.log("POST joystick", { angle, force, query });
  const res = await fetch(`${host}/joystick?q=${query}`, { method: "POST" });
  console.log("Responded joystick", await res.arrayBuffer(), await res.text());
}

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

let angle, force;

joystickManager
  .on("start,move", (event, data) => {
    angle = data.angle?.radian ?? Math.PI / 2;
    force = data.force ?? 0;
  })
  .on("start", (_event, _data) => {
    clearInterval(intervalId);
    intervalId = setInterval(() => postJoystick(angle, force), interval);
  })
  .on("end", (_event, _data) => {
    clearInterval(intervalId);
    postJoystick(angle, force);
  });

for (const button of elms.main.buttonsZone.childNodes) {
  button.onclick = async ({ target: button }) => {
    const id = button.innerText.toLowerCase();
    console.log("POST button", id);
    const res = await fetch(`${host}/${id}`, { method: "POST" });
    console.log("Responded button", await res.arrayBuffer(), await res.text());
  };
}

document.addEventListener("DOMContentLoaded", ({}) => {
  for (const input of Object.values(elms.settings)) {
    input.oninput?.({ target: input });
  }
});
