  const presentationInput = document.getElementById('presentation-image');
  const presentationPreview = document.getElementById('presentation-preview');

  const descriptionInput = document.getElementById('description-images');
  const descriptionPreview = document.getElementById('description-preview');

  presentationInput.addEventListener('change', () => {
    const file = presentationInput.files[0];
    presentationPreview.innerHTML = '';
    if (file) {
      const img = document.createElement('img');
      img.src = URL.createObjectURL(file);
      presentationPreview.appendChild(img);
    }
  });

  descriptionInput.addEventListener('change', () => {
    const files = descriptionInput.files;
    descriptionPreview.innerHTML = '';
    Array.from(files).forEach(file => {
      const img = document.createElement('img');
      img.src = URL.createObjectURL(file);
      descriptionPreview.appendChild(img);
    });
  });