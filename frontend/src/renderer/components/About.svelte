<script lang="ts">
  import ScrollContainer from "@components/ScrollContainer.svelte";
  import { CheckCircleIcon } from "svelte-feather-icons";
  import { fade, fly } from "svelte/transition";
</script>

<main>
  <div class="modal-header" in:fly={{ y: -20, duration: 400, delay: 100 }}>
    <h1 class="title">About Beyond RGB</h1>
    <p class="subtitle">Imaging Art beyond RGB</p>
  </div>
  
  <ScrollContainer maxHeight="65vh" className="body">
    <div class="content-wrapper" in:fade={{ duration: 400, delay: 200 }}>
      <!-- Overview Section -->
      <section class="info-section">
        <div class="section-header">
          <h2>Overview</h2>
        </div>
        <p class="section-text">
          Beyond RGB is an open-source software application for processing spectral
          image sets. It takes as input two RAW (or unprocessed linear TIFF) RGB
          image sets (object/flatfield/darkfield) captured under two different
          lighting or filtering conditions, and outputs:
        </p>
        <div class="feature-list">
          <div class="feature-item">
            <CheckCircleIcon size="1x" class="feature-icon" />
            <span>A color managed RGB image</span>
          </div>
          <div class="feature-item">
            <CheckCircleIcon size="1x" class="feature-icon" />
            <span>A spectral reflectance transform that enables the interactive estimation
              of reflectance curves from user-selected regions of interest in the image</span>
          </div>
          <div class="feature-item">
            <CheckCircleIcon size="1x" class="feature-icon" />
            <span>Two sets of ancillary data and plots detailing the accuracy of the color
              and spectral calibrations, respectively</span>
          </div>
        </div>
      </section>

      <!-- Design Philosophy Section -->
      <section class="info-section">
        <div class="section-header">
          <h2>Design Philosophy</h2>
        </div>
        <p class="section-text">
          Beyond RGB was designed with considerations for form, function, and user
          friendliness, and is intended for use by cultural heritage imaging
          professionals. It is cross-platform compatible and operable via an
          interactive graphical user interface, and will continue to be a living,
          updatable open-source project.
        </p>
        <div class="contact-box">
          <p class="contact-text">
            It is freely available for download from the <a href="https://beyondrgb.github.io/" target="_blank">BeyondRGB website</a>.
          </p>
          <p class="contact-email">
            Please direct comments and questions to <a href="mailto:susan.farnand@rit.edu">susan.farnand@rit.edu</a>
          </p>
        </div>
      </section>

      <!-- Development Team Section -->
      <section class="info-section team-section">
        <div class="section-header">
          <h2>Development Teams</h2>
        </div>
        <p class="section-text">
          Beyond RGB was developed by talented teams of developers across five years who dedicated their
          time and expertise to creating this powerful tool for the cultural heritage imaging community.
        </p>

        <!-- Team 1 -->
        <div class="team-year">
          <h3>Senior Project Team 1</h3>
          <div class="team-grid">
            <div class="team-member">Tristan Knox</div>
            <div class="team-member">Jacob O'Neil</div>
            <div class="team-member">Andrew Hammerstone</div>
            <div class="team-member">Jared Ponzetti</div>
            <div class="team-member">Patrick Dalesio</div>
          </div>
        </div>

        <!-- Team 2 -->
        <div class="team-year">
          <h3>Senior Project Team 2</h3>
          <div class="team-grid">
            <div class="team-member">Brendan Grau</div>
            <div class="team-member">Sungmin Kim</div>
            <div class="team-member">Keenan Miller</div>
            <div class="team-member">John Akey</div>
            <div class="team-member">Elijah Cooper</div>
          </div>
        </div>

        <!-- Team 3 -->
        <div class="team-year">
          <h3>Senior Project Team 3</h3>
          <div class="team-grid">
            <div class="team-member">Sam McQuate</div>
            <div class="team-member">Jacob Auger</div>
            <div class="team-member">Josh Greco</div>
            <div class="team-member">Robert Loos</div>
            <div class="team-member">Jonathan Pofcher</div>
          </div>
        </div>

        <!-- Team 4 -->
        <div class="team-year">
          <h3>Senior Project Team 4</h3>
          <div class="team-grid">
            <div class="team-member">Ray Axmann</div>
            <div class="team-member">Noah Pelletier</div>
            <div class="team-member">Mitchell Sulkowski</div>
            <div class="team-member">Christopher Cabonilas</div>
            <div class="team-member">Cameron Robinson</div>
            <div class="team-member">Adam Cropp</div>
          </div>
        </div>

        <!-- Team 5 -->
        <div class="team-year">
          <h3>Senior Project Team 5</h3>
          <div class="team-grid">
            <div class="team-member">Nat Manoucheri</div>
            <div class="team-member">Parker Noffke</div>
            <div class="team-member">Taylor Lineman</div>
            <div class="team-member">Raynard Miot</div>
            <div class="team-member">Jack Ganger-Spivak</div>
            <div class="team-member">Jakob Langtry</div>
          </div>
        </div>

        <p class="thank-you">
          Thank you for the time and talent you devoted to creating this tool.
          <span class="easter-egg">It really is better than Really Good Bread.</span>
        </p>
      </section>
    </div>
  </ScrollContainer>
</main>

<style lang="postcss">
  main {
    background-color: var(--color-surface);
    @apply rounded-2xl shadow-2xl overflow-hidden;
  }

  .modal-header {
    background: linear-gradient(135deg, var(--color-surface-elevated) 0%, var(--color-surface) 100%);
    @apply px-8 py-6 border-b border-gray-700/30 text-center;
  }

  .title {
    color: var(--color-text-primary);
    @apply text-3xl font-bold mb-2;
  }

  .subtitle {
    color: var(--color-text-secondary);
    @apply text-sm tracking-wider uppercase;
  }

  :global(.body) {
    @apply p-6 select-text;
  }

  .content-wrapper {
    @apply flex flex-col gap-6;
  }

  .info-section {
    background-color: var(--color-surface-elevated);
    @apply rounded-xl p-6 border border-gray-700/20;
  }

  .team-section {
    background: linear-gradient(to bottom right, var(--color-surface-elevated), var(--color-surface));
  }

  .section-header {
    color: var(--color-text-primary);
    @apply flex items-center gap-3 mb-4 pb-3 border-b border-gray-700/30;
  }

  .section-header h2 {
    @apply text-xl font-semibold;
  }

  .section-text {
    color: var(--color-text-secondary);
    @apply text-base leading-relaxed mb-4;
  }

  .feature-list {
    @apply flex flex-col gap-3 mt-4;
  }

  .feature-item {
    background-color: var(--color-surface-sunken);
    @apply flex items-start gap-3 p-4 rounded-lg border border-gray-700/20 transition-all duration-200;
  }

  .feature-item:hover {
    background-color: var(--color-interactive-hover);
    @apply border-blue-500/30 transform translate-x-1;
  }

  :global(.feature-icon) {
    color: var(--color-success);
    @apply flex-shrink-0 mt-0.5;
  }

  .feature-item span {
    color: var(--color-text-secondary);
    @apply text-sm leading-relaxed;
  }

  .contact-box {
    background-color: var(--color-surface-sunken);
    @apply p-5 rounded-lg border-l-4 border-blue-500/50 mt-4;
  }

  .contact-text {
    color: var(--color-text-secondary);
    @apply text-sm mb-2;
  }

  .contact-text a {
    color: var(--color-border-focus);
    text-decoration: underline;
    cursor: pointer;
    @apply transition-colors;
  }

  .contact-text a:hover {
    opacity: 0.8;
  }

  .contact-email {
    color: var(--color-text-primary);
    @apply text-sm;
  }

  .contact-email a {
    color: var(--color-text-primary);
    @apply font-mono bg-gray-700/30 px-2 py-1 rounded hover:text-blue-400 transition-colors;
  }

  .team-year {
    @apply mt-6 first:mt-0;
  }

  .team-year h3 {
    color: var(--color-text-secondary);
    @apply text-base font-semibold mb-3 pb-2 border-b border-gray-700/20;
  }

  .team-grid {
    @apply grid grid-cols-2 gap-3;
  }

  .team-member {
    background-color: var(--color-surface-sunken);
    color: var(--color-text-primary);
    @apply px-4 py-3 rounded-lg text-center font-medium border border-gray-700/20
           transition-all duration-200;
  }

  .team-member:hover {
    background-color: var(--color-interactive-hover);
    @apply border-blue-500/40 transform scale-105;
  }

  .thank-you {
    color: var(--color-text-secondary);
    @apply text-sm text-center italic mt-4 pt-4 border-t border-gray-700/20;
  }

  .easter-egg {
    @apply block mt-1 text-xs opacity-70;
  }

  /* Scrollbar styling now handled globally in theme.css */
</style>
