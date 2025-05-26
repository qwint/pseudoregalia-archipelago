from BaseClasses import Item, ItemClassification
from typing import NamedTuple, Dict, Set, Callable
from .constants.versions import MAP_PATCH
from .options import PseudoregaliaOptions


class PseudoregaliaItem(Item):
    game = "Pseudoregalia"


class PseudoregaliaItemData(NamedTuple):
    code: int | None = None
    frequency: int = 1
    classification: ItemClassification = ItemClassification.filler
    locked_location: Callable[[PseudoregaliaOptions], str | None] = lambda options: None
    """
    This function can return a string to indicate that the item should be placed in the corresponding location instead
    of the item pool.

    Only the first item will be locked if frequency > 1 and the rest will be placed in the item pool. If this ever needs
    to be expanded to allow locking more than one item, the function can be changed to return List[str] instead of
    str | None and PseudoregaliaWorld.create_items will need to be updated.
    """
    can_create: Callable[[PseudoregaliaOptions], bool] = lambda options: True


item_table: Dict[str, PseudoregaliaItemData] = {
    "Dream Breaker": PseudoregaliaItemData(
        code=2365810001,
        classification=ItemClassification.progression,
        locked_location=lambda options: "Dilapidated Dungeon - Dream Breaker",
        can_create=lambda options: not bool(options.progressive_breaker)),
    "Indignation": PseudoregaliaItemData(
        code=2365810002,
        classification=ItemClassification.useful),
    "Sun Greaves": PseudoregaliaItemData(
        code=2365810003,
        classification=ItemClassification.progression,
        can_create=lambda options: not bool(options.split_sun_greaves)),
    "Slide": PseudoregaliaItemData(
        code=2365810004,
        classification=ItemClassification.progression,
        can_create=lambda options: not bool(options.progressive_slide)),
    "Solar Wind": PseudoregaliaItemData(
        code=2365810005,
        classification=ItemClassification.progression,
        can_create=lambda options: not bool(options.progressive_slide)),
    "Sunsetter": PseudoregaliaItemData(
        code=2365810006,
        classification=ItemClassification.progression),
    "Strikebreak": PseudoregaliaItemData(
        code=2365810007,
        classification=ItemClassification.progression,
        can_create=lambda options: not bool(options.progressive_breaker)),
    "Cling Gem": PseudoregaliaItemData(
        code=2365810008,
        classification=ItemClassification.progression),
    "Ascendant Light": PseudoregaliaItemData(
        code=2365810009,
        classification=ItemClassification.progression),
    "Soul Cutter": PseudoregaliaItemData(
        code=2365810010,
        classification=ItemClassification.progression,
        can_create=lambda options: not bool(options.progressive_breaker)),

    "Heliacal Power": PseudoregaliaItemData(
        code=2365810011,
        classification=ItemClassification.progression,
        can_create=lambda options: not bool(options.split_sun_greaves)),
    "Aerial Finesse": PseudoregaliaItemData(
        code=2365810012,
        classification=ItemClassification.filler),
    "Pilgrimage": PseudoregaliaItemData(
        code=2365810013,
        classification=ItemClassification.filler),
    "Empathy": PseudoregaliaItemData(
        code=2365810014,
        frequency=2,
        classification=ItemClassification.filler),
    "Good Graces": PseudoregaliaItemData(
        code=2365810015,
        frequency=2,
        classification=ItemClassification.useful),
    "Martial Prowess": PseudoregaliaItemData(
        code=2365810016,
        classification=ItemClassification.useful),
    "Clear Mind": PseudoregaliaItemData(
        code=2365810017,
        frequency=3,
        classification=ItemClassification.filler),
    "Professionalism": PseudoregaliaItemData(
        code=2365810018,
        locked_location=lambda options: "Castle Sansa - Time Trial" if options.game_version == MAP_PATCH and not options.shuffle_outfits else None,
        classification=ItemClassification.filler),

    "Health Piece": PseudoregaliaItemData(
        code=2365810019,
        frequency=16,
        classification=ItemClassification.useful),
    "Small Key": PseudoregaliaItemData(
        code=2365810020,
        frequency=7,
        classification=ItemClassification.progression),

    "Major Key - Empty Bailey": PseudoregaliaItemData(
        code=2365810021,
        classification=ItemClassification.progression),
    "Major Key - The Underbelly": PseudoregaliaItemData(
        code=2365810022,
        classification=ItemClassification.progression),
    "Major Key - Tower Remains": PseudoregaliaItemData(
        code=2365810023,
        classification=ItemClassification.progression),
    "Major Key - Sansa Keep": PseudoregaliaItemData(
        code=2365810024,
        classification=ItemClassification.progression),
    "Major Key - Twilight Theatre": PseudoregaliaItemData(
        code=2365810025,
        classification=ItemClassification.progression),

    "Progressive Slide": PseudoregaliaItemData(
        code=2365810026,
        frequency=2,
        classification=ItemClassification.progression,
        can_create=lambda options: bool(options.progressive_slide)),
    "Air Kick": PseudoregaliaItemData(
        code=2365810027,
        frequency=4,
        classification=ItemClassification.progression,
        can_create=lambda options: bool(options.split_sun_greaves)),
    "Progressive Dream Breaker": PseudoregaliaItemData(
        code=2365810028,
        frequency=3,
        classification=ItemClassification.progression,
        locked_location=lambda options: "Dilapidated Dungeon - Dream Breaker",
        can_create=lambda options: bool(options.progressive_breaker)),

    "Devotion": PseudoregaliaItemData(
        code=2365810029,
        classification=ItemClassification.filler,
        locked_location=lambda options: "Dilapidated Dungeon - Time Trial" if not options.shuffle_outfits else None,
        can_create=lambda options: options.game_version == MAP_PATCH),
    "A Guardian": PseudoregaliaItemData(
        code=2365810030,
        classification=ItemClassification.filler,
        locked_location=lambda options: "Sansa Keep - Time Trial" if not options.shuffle_outfits else None,
        can_create=lambda options: options.game_version == MAP_PATCH),
    "Sweater": PseudoregaliaItemData(
        code=2365810031,
        classification=ItemClassification.filler,
        locked_location=lambda options: "Listless Library - Time Trial" if not options.shuffle_outfits else None,
        can_create=lambda options: options.game_version == MAP_PATCH),
    "Class": PseudoregaliaItemData(
        code=2365810032,
        classification=ItemClassification.filler,
        locked_location=lambda options: "Twilight Theatre - Time Trial" if not options.shuffle_outfits else None,
        can_create=lambda options: options.game_version == MAP_PATCH),
    "Chivalry": PseudoregaliaItemData(
        code=2365810033,
        classification=ItemClassification.filler,
        locked_location=lambda options: "Empty Bailey - Time Trial" if not options.shuffle_outfits else None,
        can_create=lambda options: options.game_version == MAP_PATCH),
    "Nostalgia": PseudoregaliaItemData(
        code=2365810034,
        classification=ItemClassification.filler,
        locked_location=lambda options: "The Underbelly - Time Trial" if not options.shuffle_outfits else None,
        can_create=lambda options: options.game_version == MAP_PATCH),
    "A Bleeding Heart": PseudoregaliaItemData(
        code=2365810035,
        classification=ItemClassification.filler,
        locked_location=lambda options: "Tower Remains - Time Trial" if not options.shuffle_outfits else None,
        can_create=lambda options: options.game_version == MAP_PATCH),

    "Something Worth Being Awake For": PseudoregaliaItemData(
        classification=ItemClassification.progression,
        locked_location=lambda options: "D S T RT ED M M O   Y"),
}

item_groups: Dict[str, Set[str]] = {
    "major keys": {"Major Key - Empty Bailey",
                   "Major Key - The Underbelly",
                   "Major Key - Tower Remains",
                   "Major Key - Sansa Keep",
                   "Major Key - Twilight Theatre"},
    "plunge": {"Sunsetter"},
    "air kicks": {"Sun Greaves"},
    "nike kicks": {"Sun Greaves"},
    "charge": {"Strikebreak"},
    "projectile": {"Soul Cutter"},
    "slidejump": {"Solar Wind"},
    "wallride": {"Cling Gem"},
    "pogo": {"Ascendant Light"},
    "floof": {"Professionalism"},
    "heliacal power": {"Air Kick"},
    "aspects": {"Indignation",  # some nice to have groups when sorting local/non local items in yaml etc, does not include "Memento" aka new map powerup
                "Aerial Finesse",
                "Pilgrimage",
                "Empathy",
                "Martial Prowess",
                "Clear Mind",
                "Professionalism",
                "Good Graces"},
    "mobility": {"Sun Greaves",
                 "Slide",
                 "Solar Wind",
                 "Ascendant Light",
                 "Heliacal Power",
                 "Progressive Slide",
                 "Sunsetter",
                 "Air Kick",
                 "Cling Gem"},
    "collectables": {"Health Piece",
                     "Small Key"},
    #"weapon": {"Dream Breaker",
    #           "Progressive Dream Breaker",
    #           "Strikebreak",
    #           "Soul Cutter"},
    #"attire": {"Professional", # Castle Sansa trial
    #           "Soldier", # Empty Bailey trial
    #           "Guardian", # Sansa Keep trial
    #           "Sol Sister", # Dilapidated Dungeon trial
    #           "Classy", # Twilight Theatre trial
    #           "XIX", # Underbelly trial
    #           "Sleepytime", # Listless Library trial
    #           "Bleeding Heart}, # Tower Remains trial  
}
