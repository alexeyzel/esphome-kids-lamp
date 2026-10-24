"""Ship the lamp's existing lambda helpers through ESPHome's Git loader.

No custom hardware driver or runtime component: ESPHome copies these headers
into the build and includes them before generated globals via esphome.h.
"""

import esphome.config_validation as cv

DEPENDENCIES = ["light", "display"]
CONFIG_SCHEMA = cv.Schema({})


async def to_code(config):
    pass
