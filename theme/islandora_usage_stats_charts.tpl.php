<?php
/**
 * @file
 * Template file for the Islandora Usage Stats Charts module.
 *
 * Available variables:
 *   $collapsed string
 *     The string 'collapsed' or ''.
 */
?>
<fieldset class="islandora collapsible <?php print $collapsed; ?>">
  <legend><span class="fieldset-legend"><?php print t('Usage statistics'); ?></span></legend>
  <div class="fieldset-wrapper" id="islandora-usage-stats-charts-content"><canvas id="islandora-usage-stats-chart" width="400" height="400"></canvas>
  <?php if ($csv_link): ?>
    <div id="islandora-usage-stats-charts-csv-download"><?php print $csv_link; ?></div>
  <?php endif; ?>
  </div>
</fieldset>
