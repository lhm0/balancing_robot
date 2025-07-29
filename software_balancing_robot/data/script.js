function fetchParams() {
  fetch("/params")
    .then((res) => res.json())
    .then((data) => {
      document.getElementById("kpInput").value = data.kp.toFixed(2);
      document.getElementById("kdInput").value = data.kd.toFixed(2);
    });
}

function submitParams() {
  const kp = parseFloat(document.getElementById("kpInput").value);
  const kd = parseFloat(document.getElementById("kdInput").value);

  fetch("/params", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ kp, kd }),
  })
    .then((res) => {
      if (res.ok) {
        document.getElementById("statusMsg").textContent = "Gespeichert!";
      } else {
        document.getElementById("statusMsg").textContent = "Fehler beim Speichern.";
      }
    });
}

fetchParams();