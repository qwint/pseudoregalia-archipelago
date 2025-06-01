from dataclasses import dataclass
from Options import Toggle, Choice, DefaultOnToggle, PerGameCommonOptions
from .constants.difficulties import NORMAL, HARD, EXPERT, LUNATIC
from .constants.versions import MAP_PATCH, FULL_GOLD


class LogicLevel(Choice):
    """
    The overall difficulty of the logic, used to determine the requirements to access locations and regions.

    Normal: Suitable for anyone who has beaten the game. Requires backflips and knowing where everything is.
    Hard: Requires some easier movement tricks such as cling climbing and backwards ultras using solar wind,
          and more nuanced movement like turning during an air kick.
    Expert: Requires more difficult movement tricks such as ultrahops and reverse wallkicks, and obscure knowledge.
    Lunatic: Requires extremely difficult jumps and creative thinking. No holds barred. You have been warned.
    """
    display_name = "Logic Level"
    option_normal = NORMAL
    option_hard = HARD
    option_expert = EXPERT
    option_lunatic = LUNATIC
    default = NORMAL


class ObscureLogic(Toggle):
    """
    Enables logic for obscure knowledge and creative pathing that isn't difficult to execute but may not be obvious or commonly known.
    This option is forced on if logic level is set to Expert or Lunatic.
    """
    display_name = "Obscure Logic"


class SafeSmallKeys(DefaultOnToggle):
    """
    No locked doors are in logic until all small keys are obtainable.
    Prevents potential softlocks when spending small keys out of logic.

    Currently unused.
    """
    display_name = "Safe Small Keys"


class ProgressiveBreaker(DefaultOnToggle):
    """
    Replaces Dream Breaker, Strikebreak, and Soul Cutter with three Progressive Dream Breaker items.
    """
    display_name = "Progressive Dream Breaker"


class ProgressiveSlide(DefaultOnToggle):
    """
    Replaces Slide and Solar Wind with two Progressive Slide items.
    """
    display_name = "Progressive Slide"


class SplitSunGreaves(Toggle):
    """
    Replaces Sun Greaves and Heliacal Power with four individual Air Kicks.
    """
    display_name = "Split Sun Greaves"


class GameVersion(Choice):
    """
    The version of Pseudoregalia you will use when playing the game. Different versions have different logic, locations, and items.
    After you connect, the game will warn you if the version you are playing doesn't match this option.

    map_patch: The latest version of the game. Includes time trials and new outfits.
    full_gold: Previous version, accessible using the "fullgoldjump" beta code in Steam.
    """
    display_name = "Game Version"
    option_map_patch = MAP_PATCH
    option_full_gold = FULL_GOLD
    default = MAP_PATCH


class StartWithBreaker(Toggle):
    """
    Places Dream Breaker (or one Progressive Dream Breaker) in the starting inventory.
    """
    display_name = "Start With Breaker"


class StartWithMap(Toggle):
    """
    Places the map item (Memento) in the starting inventory.

    If Full Gold version is selected, this option has no effect.
    """
    display_name = "Start With Map"


class RandomizeTimeTrials(Toggle):
    """
    Opens the time trials for randomization and puts the outfits in the item pool.
    If turned off, the time trials will not have items and the outfits are placed in the starting inventory.

    If Full Gold version is selected, this option has no effect.
    """
    display_name = "Randomize Time Trials"


@dataclass
class PseudoregaliaOptions(PerGameCommonOptions):
    game_version: GameVersion
    logic_level: LogicLevel
    obscure_logic: ObscureLogic
    progressive_breaker: ProgressiveBreaker
    progressive_slide: ProgressiveSlide
    split_sun_greaves: SplitSunGreaves
    start_with_breaker: StartWithBreaker
    start_with_map: StartWithMap
    randomize_time_trials: RandomizeTimeTrials

