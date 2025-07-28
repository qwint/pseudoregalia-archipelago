from dataclasses import dataclass
from Options import Toggle, Choice, DefaultOnToggle, PerGameCommonOptions
from .constants.difficulties import NORMAL, HARD, EXPERT, LUNATIC


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


class SplitClingGem(Toggle):
    """
    Replaces Cling Gem with three Cling Shard items.
    Each Cling Shard increases your wall run limit by two.
    """
    display_name = "Split Cling Gem"


class FullGoldSlide(Toggle):
    """
    Restores Slide to its former golden glory.
    In other words, makes it possible to perform gold ultras out of Slide before obtaining Solar Wind.
    If turned off, gold ultras are still possible with Slide after Solar Wind has been obtained by toggling it off.

    This option only effects logic on Expert+.
    """
    display_name = "Full Gold Slide"


class StartWithBreaker(Toggle):
    """
    Places Dream Breaker (or one Progressive Dream Breaker) in the starting inventory.
    """
    display_name = "Start With Breaker"


class StartWithMap(Toggle):
    """
    Places the map item (Memento) in the starting inventory.
    """
    display_name = "Start With Map"


class RandomizeTimeTrials(Toggle):
    """
    Opens the time trials for randomization and puts the outfits in the item pool.
    If turned off, the time trials will not have items and the outfits are placed in the starting inventory.
    """
    display_name = "Randomize Time Trials"


class RandomizeGoats(Toggle):
    """
    Adds goatlings as locations. Talk to the goatling to get the item.
    This option adds 19 locations.
    For each location added, an essentially useless filler item is also added to the item pool.
    """
    display_name = "Randomize Goats"


class RandomizeChairs(Toggle):
    """
    Adds places Sybil can sit as locations. Sit down to get the item.
    This option adds 16 locations.
    For each location added, an essentially useless filler item is also added to the item pool.
    """
    display_name = "Randomize Chairs"


class RandomizeBooks(Toggle):
    """
    Adds books in the library as locations. Read the book to get the item.
    This option adds 13 locations.
    For each location added, an essentially useless filler item is also added to the item pool.
    """
    display_name = "Randomize Books"


class RandomizeNotes(Toggle):
    """
    Adds notes as locations. Read the note to get the item.
    This option adds 4 locations.
    For each location added, an essentially useless filler item is also added to the item pool.
    """
    display_name = "Randomize Notes"


@dataclass
class PseudoregaliaOptions(PerGameCommonOptions):
    logic_level: LogicLevel
    obscure_logic: ObscureLogic
    progressive_breaker: ProgressiveBreaker
    progressive_slide: ProgressiveSlide
    split_sun_greaves: SplitSunGreaves
    split_cling_gem: SplitClingGem
    full_gold_slide: FullGoldSlide
    start_with_breaker: StartWithBreaker
    start_with_map: StartWithMap
    randomize_time_trials: RandomizeTimeTrials
    randomize_goats: RandomizeGoats
    randomize_chairs: RandomizeChairs
    randomize_books: RandomizeBooks
    randomize_notes: RandomizeNotes

