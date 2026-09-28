// Interações da Landing Page do DevHub
document.addEventListener('DOMContentLoaded', () => {
  // Copiar SHA-256
  const hashElement = document.getElementById('shaHash');
  if (hashElement) {
    hashElement.addEventListener('click', () => {
      const hash = hashElement.getAttribute('data-hash') || hashElement.innerText;
      navigator.clipboard.writeText(hash).then(() => {
        const originalText = hashElement.innerText;
        hashElement.innerText = "Copiado!";
        setTimeout(() => {
          hashElement.innerText = originalText;
        }, 2000);
      });
    });
  }

  // Chips do Mockup Interativo
  const chips = document.querySelectorAll('.mockup-chip');
  const rows = document.querySelectorAll('.mockup-table tbody tr');

  chips.forEach(chip => {
    chip.addEventListener('click', () => {
      chips.forEach(c => c.classList.remove('active'));
      chip.classList.add('active');

      const filter = chip.getAttribute('data-filter');
      rows.forEach(row => {
        const stack = row.getAttribute('data-stack');
        if (filter === 'ALL' || stack === filter) {
          row.style.display = '';
        } else {
          row.style.display = 'none';
        }
      });
    });
  });

  // Copiar comando terminal
  const copyBtn = document.getElementById('copyCmdBtn');
  if (copyBtn) {
    copyBtn.addEventListener('click', () => {
      const cmd = "irm https://devhub.local/install.ps1 | iex";
      navigator.clipboard.writeText(cmd).then(() => {
        copyBtn.innerText = "Copiado!";
        setTimeout(() => {
          copyBtn.innerText = "Copiar comando";
        }, 2000);
      });
    });
  }
});
