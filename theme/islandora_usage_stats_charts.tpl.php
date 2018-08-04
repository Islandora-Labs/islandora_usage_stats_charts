<?php
/**
 * @file
 * Template file for the Islandora Usage Stats Charts module.
 *
 * Available variables:
 *   $collapsed string
 *     The string 'collapsed' or ''.
 *   $content string
 *     The content of the islandora-usage-stats-charts-content div.
 *   $csv_link string
 *     The link to the CSV file.
 */
?>
<fieldset class="islandora collapsible <?php print $collapsed; ?>">
  <legend><span class="fieldset-legend"><?php print t('Usage statistics'); ?></span></legend>
  <div class="fieldset-wrapper"><div id="islandora-usage-stats-charts-content"><?php print $content; ?></div>
  <?php if ($csv_link): ?>
    <div id="islandora-usage-stats-charts-csv-download"><?php print $csv_link; ?></div>
  <?php endif; ?>
  </div>
</fieldset>
