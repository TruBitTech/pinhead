// Matrix demo
const matrix = document.getElementById('matrix');
const states = ['none', 'white', 'red', 'green'];
const grid = Array(16).fill(0).map(() => Array(16).fill(0));

function renderMatrix() {
  matrix.innerHTML = '';
  for (let r = 0; r < 16; r++) {
    for (let c = 0; c < 16; c++) {
      const cell = document.createElement('div');
      cell.className = 'cell ' + states[grid[r][c]];
      cell.onclick = () => {
        grid[r][c] = (grid[r][c] + 1) % 4;
        renderMatrix();
      };
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
  }, 80);
});

// Touch support
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
  }, 80);
});
