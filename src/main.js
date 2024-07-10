import nipplejs from "nipplejs";
import "sanitize.css";
import "./style.css";

const elms = {
  body: document.querySelector("body"),
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

const joystickPoster = {
  _angle: Math.PI / 2,
  _force: 1,
  get angle() {
    return this._angle;
  },
  get force() {
    return this._force;
  },
  get query() {
    return [
      Math.round((this.angle * 128) / Math.PI + (this.force >= 0 ? 256 : 384)) % 256,
      Math.min(Math.abs(Math.floor(this.force * 255)), 255),
    ]
      .map((num) => num.toString(16).toUpperCase().padStart(2, "0"))
      .join("");
  },

  async post() {
    console.log("POST joystick", { angle: this.angle, force: this.force, query: this.query });
    try {
      const res = await fetch(`${host}/joystick?q=${this.query}`, { method: "POST" });
      console.log("Responded joystick", await res.arrayBuffer(), await res.text());
      return res;
    } catch (error) {
      console.error(error);
      return null;
    }
  },

  intervalId: null,

  update({ angle, force } = {}) {
    this._angle = angle ?? Math.PI / 2;
    this._force = force ?? 1;
    if (this.intervalId) return false;
    this.intervalId = setInterval(() => this.post(), interval);
    return true;
  },
  stop() {
    if (!this.intervalId) return false;
    clearInterval(this.intervalId);
    this.intervalId = null;
    this.post();
    return true;
  },
};

/// settings

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

document.addEventListener("DOMContentLoaded", ({}) => {
  for (const input of Object.values(elms.settings)) input.oninput?.({ target: input });
});

/// gamepad

// joystick

const joystickManager = nipplejs.create({
  zone: elms.main.joystickZone,
  color: "#55CCFF",
  size: 160,
  mode: "dynamic",
});

joystickManager
  .on("move", (_event, data) => joystickPoster.update({ angle: data.angle?.radian, force: data.force }))
  .on("end", (_event, _data) => joystickPoster.stop());

// buttons

for (const button of elms.main.buttonsZone.childNodes) {
  button.onclick = async ({ target: button }) => {
    const id = button.innerText.toLowerCase();
    console.log("POST button", id);
    const res = await fetch(`${host}/${id}`, { method: "POST" });
    console.log("Responded button", await res.arrayBuffer(), await res.text());
  };
}

// keyboard

const pressing = Object.fromEntries(
  ["arrowright", "arrowup", "arrowleft", "arrowdown", "d", "w", "a", "s", "shift"].map((key) => [key, false]),
);

function keyUpdate() {
  let x = 0;
  let y = 0;
  if (pressing.arrowright || pressing.d) x += 1;
  if (pressing.arrowup || pressing.w) y += 1;
  if (pressing.arrowleft || pressing.a) x -= 1;
  if (pressing.arrowdown || pressing.s) y -= 1;
  if (x === 0 && y === 0) joystickPoster.stop();
  else joystickPoster.update({ angle: Math.atan2(y, x), force: pressing.shift ? 0.5 : 1 });
}

elms.body.onkeydown = ({ target, key }) => {
  key = key.toLowerCase();
  if (target.nodeName === "input" || typeof pressing[key] !== "boolean") return;
  pressing[key] = true;
  keyUpdate();
};

elms.body.onkeyup = ({ key }) => {
  key = key.toLowerCase();
  if (typeof pressing[key] !== "boolean") return;
  pressing[key] = false;
  keyUpdate();
};
