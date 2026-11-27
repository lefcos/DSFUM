import json
import os
import time
import serial
import serial.tools.list_ports

JSON_FILE = "stoc_magazin.json"
BAUD_RATE = 9600

PRODUSE_INITIALE = [
    {"name": "Lapte ",     "price": 6.50,  "stock": 10},
    {"name": "Paine ",        "price": 4.00,  "stock": 15},
    {"name": "Oua ",    "price": 12.00, "stock": 8},
    {"name": "Mere ",     "price": 5.50,  "stock": 20},
    {"name": "Cascaval ", "price": 14.25, "stock": 5},
    {"name": "Cafea ",   "price": 22.00, "stock": 6},
    {"name": "Unt ",     "price": 11.50, "stock": 9},
]


def gaseste_port_arduino():
    """Detecteaza automat portul Arduino pe Linux (/dev/ttyACM* sau /dev/ttyUSB*)."""
    porturi = list(serial.tools.list_ports.comports())
    for p in porturi:
        if "ACM" in p.device or "USB" in p.device:
            return p.device
    return "/dev/ttyACM0"


def incarca_stoc():
    if os.path.exists(JSON_FILE):
        with open(JSON_FILE, "r", encoding="utf-8") as f:
            date = json.load(f)
    else:
        date = PRODUSE_INITIALE
        salveaza_stoc(date)

    # Resetam cosul la 0 la fiecare pornire a sesiunii de cumparaturi
    for p in date:
        p["cart"] = 0
    return date


def salveaza_stoc(inventory):
    # Salvam in JSON doar nume, pret si stocul ramas
    date_de_salvat = [
        {"name": p["name"], "price": p["price"], "stock": p["stock"]}
        for p in inventory
    ]
    with open(JSON_FILE, "w", encoding="utf-8") as f:
        json.dump(date_de_salvat, f, indent=2)


def actualizeaza_lcd(ser, inventory, idx, arata_total):
    if arata_total:
        suma_totala = sum(p["price"] * p["cart"] for p in inventory)
        total_produse = sum(p["cart"] for p in inventory)
        linie1 = f"L1:Produse cos: {total_produse}\n"
        linie2 = f"L2:Total:{suma_totala:6.2f}lei\n"
    else:
        produs = inventory[idx]
        # Formatare fixa pe 16 caractere pentru ecranul LCD 16x2
        linie1 = f"L1:{produs['name']:<11}{produs['price']:5.2f}\n"
        linie2 = f"L2:Stoc:{produs['stock']:<3} Cos:{produs['cart']}\n"

    ser.write(linie1.encode("ascii"))
    time.sleep(0.05)
    ser.write(linie2.encode("ascii"))

    print(f"[LCD] {linie1.strip()} | {linie2.strip()}")


def main():
    port = gaseste_port_arduino()
    inventory = incarca_stoc()
    idx = 0
    arata_total = False

    print(f"Conectare la Arduino pe {port} ({BAUD_RATE} baud)...")
    try:
        ser = serial.Serial(port, BAUD_RATE, timeout=1)
    except serial.SerialException as e:
        print(f"Eroare la deschiderea portului {port}: {e}")
        print("Verifica sa ai Serial Monitor INCHIS in Arduino IDE!")
        return

    time.sleep(2)  # Asteapta reset-ul placii Arduino la conectare
    actualizeaza_lcd(ser, inventory, idx, arata_total)
    print("Gata! Apasa butoanele de pe shield (Ctrl+C pentru oprire).\n")

    try:
        while True:
            mesaj = ser.readline().decode("ascii", errors="ignore").strip()
            if not mesaj:
                continue

            print(f"[ARDUINO -> PC] Buton: {mesaj}")

            if mesaj == "UP":
                arata_total = False
                idx = (idx - 1) % len(inventory)
                actualizeaza_lcd(ser, inventory, idx, arata_total)

            elif mesaj == "DOWN":
                arata_total = False
                idx = (idx + 1) % len(inventory)
                actualizeaza_lcd(ser, inventory, idx, arata_total)

            elif mesaj == "RIGHT":
                arata_total = False
                if inventory[idx]["stock"] > 0:
                    inventory[idx]["stock"] -= 1
                    inventory[idx]["cart"] += 1
                    salveaza_stoc(inventory)
                actualizeaza_lcd(ser, inventory, idx, arata_total)

            elif mesaj == "LEFT":
                arata_total = False
                if inventory[idx]["cart"] > 0:
                    inventory[idx]["cart"] -= 1
                    inventory[idx]["stock"] += 1
                    salveaza_stoc(inventory)
                actualizeaza_lcd(ser, inventory, idx, arata_total)

            elif mesaj == "SELECT":
                arata_total = not arata_total
                actualizeaza_lcd(ser, inventory, idx, arata_total)

    except KeyboardInterrupt:
        print("\nOprire script. Stocul a fost salvat in stoc_magazin.json.")
    finally:
        ser.close()


if __name__ == "__main__":
    main()