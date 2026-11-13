#pragma once

// WS2812B LED strip on 3-wire port C

// Starts the running red and white pattern in its own task.
// Call once from initialize().
void startLeds();
