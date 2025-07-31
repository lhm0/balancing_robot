// Parameter laden
function fetchParams() {
  fetch("/params")
    .then((res) => res.json())
    .then((data) => {
      document.getElementById("kpInput").value = data.kp.toFixed(2);
      document.getElementById("kiInput").value = data.ki.toFixed(2);
      document.getElementById("kdInput").value = data.kd.toFixed(2);
    })
    .catch((err) => {
      console.error("Fehler beim Laden der Parameter:", err);
    });
}

// Parameter speichern
function submitParams() {
  const kp = parseFloat(document.getElementById("kpInput").value);
  const ki = parseFloat(document.getElementById("kiInput").value);
  const kd = parseFloat(document.getElementById("kdInput").value);

  fetch("/params", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ kp, ki, kd}),
  })
    .then((res) => {
      const msg = res.ok ? "Gespeichert!" : "Fehler beim Speichern.";
      document.getElementById("statusMsg").textContent = msg;
    })
    .catch((err) => {
      document.getElementById("statusMsg").textContent = "Netzwerkfehler.";
      console.error("POST Fehler:", err);
    });
}

// Winkelanzeige aktualisieren
function fetchAngle() {
  fetch("/angle")
    .then((res) => res.json())
    .then((data) => {
      document.getElementById("angleDisplay").textContent = data.angle.toFixed(2);
    })
    .catch((err) => {
      console.warn("Fehler beim Abrufen des Winkels:", err);
    });
}

function resetPosition() {
  fetch("/resetPosition", { method: "POST" })
    .then((res) => {
      const msg = res.ok ? "Position zurückgesetzt." : "Fehler beim Zurücksetzen.";
      document.getElementById("statusMsg").textContent = msg;
    })
    .catch((err) => {
      document.getElementById("statusMsg").textContent = "Netzwerkfehler.";
      console.error("Reset Fehler:", err);
    });
}

function adjustOffset(delta) {
  fetch("/offset", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ delta: delta })
  })
  .then(res => {
    if (!res.ok) throw new Error("Offset-Update fehlgeschlagen");
  })
  .catch(err => {
    console.error("Fehler beim Setzen des Offsets:", err);
  });
}

// Initialer Aufruf & Wiederholungen starten
fetchParams();
setInterval(fetchAngle, 200); // alle 200 ms aktualisieren