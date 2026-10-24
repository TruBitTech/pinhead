// Intersection matrix
const matrix = document.getElementById('matrix');
const states = ['none', 'white', 'red', 'green'];
const grid = Array(16).fill(0).map(() => Array(16).fill(0));

const colLabels = ['A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P'];
const rowLabels = ['1','2','3','4','5','6','7','8','9','10','11','12','13','14','15','16'];

function renderMatrix() {
  matrix.innerHTML = '';

  // Corner empty
  const corner = document.createElement('div');
  corner.className = 'matrix-header';
  matrix.appendChild(corner);

  // Column headers
  for (let c = 0; c < 16; c++) {
    const h = document.createElement('div');
    h.className = 'matrix-header';
    h.textContent = colLabels[c];
    matrix.appendChild(h);
  }

  // Rows
  for (let r = 0; r < 16; r++) {
    const rh = document.createElement('div');
    rh.className = 'matrix-row-header';
    rh.textContent = rowLabels[r];
    matrix.appendChild(rh);

    for (let c = 0; c < 16; c++) {
      const cell = document.createElement('div');
      cell.className = 'matrix-cell';

      const pin = document.createElement('div');
      pin.className = 'pin ' + states[grid[r][c]];
      pin.dataset.r = r;
      pin.dataset.c = c;
      pin.onclick = (e) => {
        e.stopPropagation();
        grid[r][c] = (grid[r][c] + 1) % 4;
        renderMatrix();
      };
      cell.appendChild(pin);
      matrix.appendChild(cell);
    }
  }
}
renderMatrix();

// Sweepable keys
const keysContainer = document.getElementById('keys');
const keyCount = 27;
let isDragging = false;
let lastKey = -1;

for (let i = 0; i < keyCount; i++) {
  const key = document.createElement('div');
  key.className = 'key' + (i % 2 === 0 ? ' white' : '');
  key.dataset.index = i;
  keysContainer.appendChild(key);
}

function activateKey(index) {
  if (index === lastKey) return;
  document.querySelectorAll('.key').forEach(k => k.classList.remove('active'));
  if (index >= 0 && index < keyCount) {
    keysContainer.children[index].classList.add('active');
    lastKey = index;
  }
}

keysContainer.addEventListener('mousedown', (e) => {
  isDragging = true;
  const key = e.target.closest('.key');
  if (key) activateKey(+key.dataset.index);
});

keysContainer.addEventListener('mousemove', (e) => {
  if (!isDragging) return;
  const key = e.target.closest('.key');
  if (key) activateKey(+key.dataset.index);
});

document.addEventListener('mouseup', () => {
  isDragging = false;
  setTimeout(() => {
    document.querySelectorAll('.key').forEach(k => k.classList.remove('active'));
    lastKey = -1;
  }, 60);
});

keysContainer.addEventListener('touchstart', (e) => {
  isDragging = true;
  const touch = e.touches[0];
  const key = document.elementFromPoint(touch.clientX, touch.clientY)?.closest('.key');
  if (key) activateKey(+key.dataset.index);
  e.preventDefault();
}, { passive: false });

keysContainer.addEventListener('touchmove', (e) => {
  if (!isDragging) return;
  const touch = e.touches[0];
  const key = document.elementFromPoint(touch.clientX, touch.clientY)?.closest('.key');
  if (key) activateKey(+key.dataset.index);
  e.preventDefault();
}, { passive: false });

document.addEventListener('touchend', () => {
  isDragging = false;
  setTimeout(() => {
    document.querySelectorAll('.key').forEach(k => k.classList.remove('active'));
    lastKey = -1;
  }, 60);
});
