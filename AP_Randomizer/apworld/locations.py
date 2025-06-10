from BaseClasses import Location
from typing import NamedTuple, Callable
from .constants.versions import MAP_PATCH, FULL_GOLD
from .options import PseudoregaliaOptions


class PseudoregaliaLocation(Location):
    game = "Pseudoregalia"


class PseudoregaliaLocationData(NamedTuple):
    region: str
    code: int | None = None
    can_create: Callable[[PseudoregaliaOptions], bool] = lambda options: True
    locked_item: str | None = None


location_table = {
    # Sorted by greater region, then subregion
    # Then abilities first
    # Then alphabetically
    # Anything optional goes below the 50 base locations

    "Dilapidated Dungeon - Dream Breaker": PseudoregaliaLocationData(
        code=2365810001,
        region="Dungeon Mirror"),
    "Dilapidated Dungeon - Slide": PseudoregaliaLocationData(
        code=2365810002,
        region="Dungeon Slide"),
    "Dilapidated Dungeon - Alcove Near Mirror": PseudoregaliaLocationData(
        code=2365810003,
        region="Dungeon => Castle",),
    "Dilapidated Dungeon - Dark Orbs": PseudoregaliaLocationData(
        code=2365810004,
        region="Dungeon Escape Upper",),
    "Dilapidated Dungeon - Past Poles": PseudoregaliaLocationData(
        code=2365810005,
        region="Dungeon Strong Eyes",),
    "Dilapidated Dungeon - Rafters": PseudoregaliaLocationData(
        code=2365810006,
        region="Dungeon Strong Eyes",),
    "Dilapidated Dungeon - Strong Eyes": PseudoregaliaLocationData(
        code=2365810007,
        region="Dungeon Strong Eyes",),

    "Castle Sansa - Indignation": PseudoregaliaLocationData(
        code=2365810008,
        region="Castle Main"),
    "Castle Sansa - Alcove Near Dungeon": PseudoregaliaLocationData(
        code=2365810009,
        region="Castle => Theatre Pillar",),
    "Castle Sansa - Balcony": PseudoregaliaLocationData(
        code=2365810010,
        region="Castle Main",),
    "Castle Sansa - Corner Corridor": PseudoregaliaLocationData(
        code=2365810011,
        region="Castle Main",),
    "Castle Sansa - Floater In Courtyard": PseudoregaliaLocationData(
        code=2365810012,
        region="Castle Main",),
    "Castle Sansa - Locked Door": PseudoregaliaLocationData(
        code=2365810013,
        region="Castle Main",
        can_create=lambda options: options.game_version == FULL_GOLD),
    "Castle Sansa - Platform In Main Halls": PseudoregaliaLocationData(
        code=2365810014,
        region="Castle Main",),
    "Castle Sansa - Tall Room Near Wheel Crawlers": PseudoregaliaLocationData(
        code=2365810015,
        region="Castle Main",),
    "Castle Sansa - Wheel Crawlers": PseudoregaliaLocationData(
        code=2365810016,
        region="Castle Main",),
    "Castle Sansa - High Climb From Courtyard": PseudoregaliaLocationData(
        code=2365810017,
        region="Castle High Climb",),
    "Castle Sansa - Alcove Near Scythe Corridor": PseudoregaliaLocationData(
        code=2365810018,
        region="Castle By Scythe Corridor",),
    "Castle Sansa - Near Theatre Front": PseudoregaliaLocationData(
        code=2365810019,
        region="Castle Moon Room",),

    "Sansa Keep - Strikebreak": PseudoregaliaLocationData(
        code=2365810020,
        region="Keep Main"),
    "Sansa Keep - Alcove Near Locked Door": PseudoregaliaLocationData(
        code=2365810021,
        region="Keep Locked Room",),
    "Sansa Keep - Levers Room": PseudoregaliaLocationData(
        code=2365810022,
        region="Keep Main",),
    "Sansa Keep - Lonely Throne": PseudoregaliaLocationData(
        code=2365810023,
        region="Keep Throne Room",),
    "Sansa Keep - Near Theatre": PseudoregaliaLocationData(
        code=2365810024,
        region="Keep Main",),
    "Sansa Keep - Sunsetter": PseudoregaliaLocationData(
        code=2365810025,
        region="Keep Sunsetter"),

    "Listless Library - Sun Greaves": PseudoregaliaLocationData(
        code=2365810026,
        region="Library Greaves",
        can_create=lambda options: not bool(options.split_sun_greaves)),
    "Listless Library - Upper Back": PseudoregaliaLocationData(
        code=2365810027,
        region="Library Top",),
    "Listless Library - Locked Door Across": PseudoregaliaLocationData(
        code=2365810028,
        region="Library Locked",),
    "Listless Library - Locked Door Left": PseudoregaliaLocationData(
        code=2365810029,
        region="Library Locked",),

    "Twilight Theatre - Soul Cutter": PseudoregaliaLocationData(
        code=2365810030,
        region="Theatre Main"),
    "Twilight Theatre - Back Of Auditorium": PseudoregaliaLocationData(
        code=2365810031,
        region="Theatre Main",),
    "Twilight Theatre - Center Stage": PseudoregaliaLocationData(
        code=2365810032,
        region="Theatre Main",),
    "Twilight Theatre - Locked Door": PseudoregaliaLocationData(
        code=2365810033,
        region="Theatre Main",),
    "Twilight Theatre - Murderous Goat": PseudoregaliaLocationData(
        code=2365810034,
        region="Theatre Main",),
    "Twilight Theatre - Corner Beam": PseudoregaliaLocationData(
        code=2365810035,
        region="Theatre Pillar",),

    "Empty Bailey - Solar Wind": PseudoregaliaLocationData(
        code=2365810036,
        region="Bailey Lower",),
    "Empty Bailey - Center Steeple": PseudoregaliaLocationData(
        code=2365810037,
        region="Bailey Upper",),
    "Empty Bailey - Cheese Bell": PseudoregaliaLocationData(
        code=2365810038,
        region="Bailey Upper",),
    "Empty Bailey - Guarded Hand": PseudoregaliaLocationData(
        code=2365810039,
        region="Bailey Lower",),
    "Empty Bailey - Inside Building": PseudoregaliaLocationData(
        code=2365810040,
        region="Bailey Lower",),

    "The Underbelly - Ascendant Light": PseudoregaliaLocationData(
        code=2365810041,
        region="Underbelly Ascendant Light"),
    "The Underbelly - Alcove Near Light": PseudoregaliaLocationData(
        code=2365810042,
        region="Underbelly Light Pillar",),
    "The Underbelly - Building Near Little Guy": PseudoregaliaLocationData(
        code=2365810043,
        region="Underbelly => Bailey",),
    "The Underbelly - Locked Door": PseudoregaliaLocationData(
        code=2365810044,
        region="Underbelly By Heliacal",),
    "The Underbelly - Main Room": PseudoregaliaLocationData(
        code=2365810045,
        region="Underbelly Main Upper",),
    "The Underbelly - Rafters Near Keep": PseudoregaliaLocationData(
        code=2365810046,
        region="Underbelly => Keep",),
    "The Underbelly - Strikebreak Wall": PseudoregaliaLocationData(
        code=2365810047,
        region="Underbelly Main Upper",),
    "The Underbelly - Surrounded By Holes": PseudoregaliaLocationData(
        code=2365810048,
        region="Underbelly Hole",),

    "Tower Remains - Cling Gem": PseudoregaliaLocationData(
        code=2365810049,
        region="Tower Remains"),
    "Tower Remains - Atop The Tower": PseudoregaliaLocationData(
        code=2365810050,
        region="The Great Door",),

    "Listless Library - Sun Greaves 1": PseudoregaliaLocationData(
        code=2365810051,
        region="Library Greaves",
        can_create=lambda options: bool(options.split_sun_greaves)),
    "Listless Library - Sun Greaves 2": PseudoregaliaLocationData(
        code=2365810052,
        region="Library Greaves",
        can_create=lambda options: bool(options.split_sun_greaves)),
    "Listless Library - Sun Greaves 3": PseudoregaliaLocationData(
        code=2365810053,
        region="Library Greaves",
        can_create=lambda options: bool(options.split_sun_greaves)),
    
    "Dilapidated Dungeon - Time Trial": PseudoregaliaLocationData(
        code=2365810054,
        region="Dungeon Mirror",
        can_create=lambda options: options.game_version == MAP_PATCH and options.randomize_time_trials),
    "Castle Sansa - Time Trial": PseudoregaliaLocationData(
        code=2365810055,
        region="Castle Main",
        can_create=lambda options: options.game_version == MAP_PATCH and options.randomize_time_trials),
    "Sansa Keep - Time Trial": PseudoregaliaLocationData(
        code=2365810056,
        region="Keep Throne Room",
        can_create=lambda options: options.game_version == MAP_PATCH and options.randomize_time_trials),
    "Listless Library - Time Trial": PseudoregaliaLocationData(
        code=2365810057,
        region="Library Main",
        can_create=lambda options: options.game_version == MAP_PATCH and options.randomize_time_trials),
    "Twilight Theatre - Time Trial": PseudoregaliaLocationData(
        code=2365810058,
        region="Theatre Pillar",
        can_create=lambda options: options.game_version == MAP_PATCH and options.randomize_time_trials),
    "Empty Bailey - Time Trial": PseudoregaliaLocationData(
        code=2365810059,
        region="Bailey Upper",
        can_create=lambda options: options.game_version == MAP_PATCH and options.randomize_time_trials),
    "The Underbelly - Time Trial": PseudoregaliaLocationData(
        code=2365810060,
        region="Underbelly Main Upper",
        can_create=lambda options: options.game_version == MAP_PATCH and options.randomize_time_trials),
    "Tower Remains - Time Trial": PseudoregaliaLocationData(
        code=2365810061,
        region="The Great Door",
        can_create=lambda options: options.game_version == MAP_PATCH and options.randomize_time_trials),

    "Castle Sansa - Memento": PseudoregaliaLocationData(
        code=2365810062,
        region="Castle Main",
        can_create=lambda options: options.game_version == MAP_PATCH),

    "D S T RT ED M M O   Y": PseudoregaliaLocationData(
        region="The Great Door",
        locked_item="Something Worth Being Awake For"),
}
