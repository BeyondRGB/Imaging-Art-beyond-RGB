<script lang="ts">
     // Optional: you can receive closeModal as a prop
     export let closeModal = () => {};
   </script>
   
<main>
    <div class="infomodal">
        <h2>
            Not enough recognizable image filename suffixes were found, <br>the program will attempt to sort by name but the selected roles may not be accurate.<br>
                Try renaming your images to match the guide on this page for better sorting results.
        </h2>
        <h2>Image file naming conventions:</h2>
        <p class="section">
            Filename format: anyFilename-typeIndicator_imageID.extension
        </p>
        <p class="section">
            Where type indicator is one of the types listed below and imageID is A or B
        </p>
        <p>
            Image: "image", "print", "object", "art", "exhibit", "paint", "subject", "item", "obj"<br>
            Color Target: "target", "targets", "color", "grid", "map", "passport"<br>
            Flatfield: "flatfield", "flat", "white", "ff", "vignetting", "vignette", "light", "correction", "f", "w"<br>
            Darkfield: "darkfield", "d", "darkcurrent", "dc", "black", "dark", "current", "signal", ”internal", "camera"<br>
            
        </p>
        <p class="section">
            <!-- <button class="closeDia" autofocus on:click={closeModal}>Confirm</button> -->
        </p>
    </div>
    
</main>
<style lang="postcss">
    main {
    @apply w-[600px] max-h-[80vh] bg-gray-800 rounded-xl shadow-2xl overflow-hidden;
  }

  .infomodal {
    @apply flex flex-col p-8 gap-8 overflow-y-auto;
  }

  h1 {
    @apply text-2xl font-semibold text-gray-100 mb-2;
  }

  .section {
    @apply flex flex-col gap-4;
  }

  h2 {
    @apply text-sm font-medium text-gray-400 uppercase tracking-wider mb-2;
  }
</style>