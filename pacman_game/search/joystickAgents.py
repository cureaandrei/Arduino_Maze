# joystickAgents.py
import time
import serial
import serial.tools.list_ports

from game import Directions
from game import Agent


BAUD = 115200

FORCE_PORT = None

DEAD = 140
HYST = 40
CENTER = 512

LOG_FILE = "telemetry_log.txt"
LOG_FLUSH_EVERY = 10


def auto_find_port():
    ports = list(serial.tools.list_ports.comports())
    for p in ports:
        desc = (p.description or "").lower()
        if ("arduino" in desc) or ("uno" in desc) or ("ch340" in desc) or ("cp210" in desc) or ("usb-serial" in desc):
            return p.device
    if len(ports) == 1:
        return ports[0].device
    raise RuntimeError("Nu pot detecta portul. Porturi: " + ", ".join([pp.device + " " + (pp.description or "") for pp in ports]))


def read_dir_from_joystick(x, y):
    dx = x - CENTER
    dy = y - CENTER

    if abs(dx) < DEAD and abs(dy) < DEAD:
        return None

    if abs(dx) > abs(dy) + HYST:
        return Directions.EAST if dx > 0 else Directions.WEST
    if abs(dy) > abs(dx) + HYST:
        return Directions.NORTH if dy < 0 else Directions.SOUTH

    return None


class JoystickAgent(Agent):
    def __init__(self):
        port = FORCE_PORT if FORCE_PORT else auto_find_port()
        self.ser = serial.Serial(port, BAUD, timeout=0.1)
        time.sleep(2)

        self.current = Directions.STOP
        self.desired = Directions.STOP

        self.last_change_t = 0.0
        self.change_cooldown = 0.05
        self.log = open(LOG_FILE, "w", encoding="utf-8")
        self.log.write("ts,pac_x,pac_y,score,desired,current\n")
        self.log.flush()

        self._step = 0

    def _pump_serial(self):
        last_dir = None
        while self.ser.in_waiting:
            line = self.ser.readline().decode(errors="ignore").strip()
            if not line:
                continue
            try:
                x, y, b = map(int, line.split(","))
            except:
                continue

            d = read_dir_from_joystick(x, y)
            if d is not None:
                last_dir = d
        return last_dir

    def _log_state(self, state):
        try:
            (px, py) = state.getPacmanPosition()
        except:
            px, py = -1, -1
        try:
            score = state.getScore()
        except:
            score = 0

        self.log.write(f"{time.time():.6f},{px},{py},{score},{self.desired},{self.current}\n")
        self._step += 1
        if self._step % LOG_FLUSH_EVERY == 0:
            self.log.flush()

    def getAction(self, state):
        d = self._pump_serial()
        now = time.time()
        if d is not None and (now - self.last_change_t) >= self.change_cooldown:
            self.desired = d
            self.last_change_t = now

        legal = state.getLegalActions(0)

        if self.desired in legal:
            self.current = self.desired
            self._log_state(state)
            return self.current

        if self.current in legal:
            self._log_state(state)
            return self.current

        self.current = Directions.STOP
        self._log_state(state)
        return Directions.STOP
