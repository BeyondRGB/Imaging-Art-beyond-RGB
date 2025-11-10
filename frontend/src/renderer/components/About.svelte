<script lang="ts">
  import ScrollContainer from "@components/ScrollContainer.svelte";
</script>

<main>
  <div class="title">About</div>
  <ScrollContainer maxHeight="70vh" className="body">
    <p>
      Beyond RGB is an open-source software application for processing spectral
      image sets. It takes as input two RAW (or unprocessed linear TIFF) RGB
      image sets (object/flatfield/darkfield) captured under two different
      lighting or filtering conditions, and outputs
    </p>
    <ol>
      <li>a color managed RGB image</li>
      <li>
        a spectral reflectance transform that enables the interactive estimation
        of reflectance curves from user-selected regions of interest in the
        image
      </li>
      <li>
        two sets of ancillary data and plots detailing the accuracy of the color
        and spectral calibrations, respectively
      </li>
    </ol>
    <br />
    <p>
      Beyond RGB was designed with considerations for form, function, and user
      friendliness, and is intended for use by cultural heritage imaging
      professionals. It is cross-platform compatible and operable via an
      interactive graphical user interface, and will continue to be a living,
      updatable open-source project. It is freely available for download from
      the RIT Munsell Color Science Laboratory website. Please direct comments
      and questions to susan.farnand at rit.edu.
    </p>
    <br />
    <p>
      The Quick Start Guide provides instructions on getting started with Beyond
      RGB. It gives an overview of the steps involved in
    </p>
    <ol>
      <li>
        setting up the software to import and process the spectral image stack
      </li>
      <li>
        inspecting and interacting with the output image and other files. There
        are also written cues describing the action to take on each screen
        within the application as it is navigated
      </li>
    </ol>
    <br />
    <p>
      Beyond RGB was developed by Patrick Dalesio, Andrew Hammerstone, Tristan
      Knox, Jake O’Neil, and Jared Ponzetti. Thank you for the time and talent
      you devoted to creating this tool. It really is better than Really Good
      Bread.
    </p>
  </ScrollContainer>
</main>

<style lang="postcss">
  main {
    background-color: var(--color-surface);
    @apply rounded-xl;
  }
  .title {
    background-color: var(--color-surface-elevated);
    @apply text-2xl p-2;
  }
  :global(.body) {
    @apply p-4 flex flex-col gap-2 select-text;
  }
  ol {
    list-style: decimal;
    list-style-position: outside;
    background-color: var(--color-surface-elevated);
    @apply pl-8 py-2 pr-2 rounded-xl;
  }
  li {
    @apply p-1;
  }
  p {
    @apply text-base;
  }

  /* Scrollbar styling now handled globally in theme.css */
</style>
