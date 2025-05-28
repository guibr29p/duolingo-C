
function toggleContent(id) {
  
    const section = document.getElementById(id);

 
    const isVisible = section.style.display === 'block';

    
    const allSections = document.querySelectorAll('.content');

    
    allSections.forEach(section => section.style.display = 'none');

  
    if (!isVisible) {
        section.style.display = 'block';
    }
}
