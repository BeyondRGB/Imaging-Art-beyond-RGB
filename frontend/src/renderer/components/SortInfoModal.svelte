 <main>
    <div class="infomodal">
        <h1>
            Not enough recognizable image filename suffixes were found, the program will attempt to sort by name but the selected roles may not be accurate.
                
        </h1>
        <p class="section">
          Try renaming your images to match the guide on this page for better sorting results.
        </p>
        <div class="mono_text">anyFilename-typeIndicator_ImageID.extension</div>
        <p class="section">
            anyFilename can be any identifier you want.<br>
            
            ImageID can also be anything but must match with the associated images, i.e. the associated object, color target, flatfield, and darkfield images should all have the same ImageID. Use A or B if you are unsure.<br>

            Type indicator is one of the types listed below.
        </p>
        <p>
            Image: "image", "print", "object", "art", "exhibit", "paint", "subject", "item", "obj"<br>
            Color Target: "target", "targets", "color", "grid", "map", "passport"<br>
            Flatfield: "flatfield", "flat", "white", "ff", "vignetting", "vignette", "light", "correction", "f", "w"<br>
            Darkfield: "darkfield", "d", "darkcurrent", "dc", "black", "dark", "current", "signal", ”internal", "camera"<br>
            
        </p>
        
    </div>
    
</main>
<style lang="postcss">
    main {
    background-color: var(--color-surface-elevated);
    @apply w-[600px] max-h-[80vh] rounded-xl shadow-2xl overflow-hidden;
  }

  h1 {
    color: var(--color-text-primary);
    @apply text-xl font-semibold mb-2;
  }

  p {
    color: var(--color-text-secondary);
  }

  .infomodal {
    @apply flex flex-col p-8 gap-8 overflow-y-auto;
  }

  .section {
    @apply flex flex-col gap-4;
  }

  .mono_text {
    background-color: var(--color-surface-sunken);
    color: var(--color-text-primary);
    border: 1px solid var(--color-border);
    @apply font-mono flex flex-col gap-4 text-center p-2 rounded-lg;
  }
</style>