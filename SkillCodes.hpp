#ifndef __H_SKILL_CODES__
#define __H_SKILL_CODES

/** This file contains the numeric value mapping to skill code names. **/

#include <map>
#include <string>

// A Map containing all skill hex code mappings to their string name
static const std::map<unsigned int, std::string> skill_map = {
  // General Actions
  {0x0003, "Sprint"},

  /** BEGIN TANK SECTION **/
  // Tank Role Actions
  {0x1d6b, "Rampart"},
  {0x1d6d, "Provoke"},
  {0x1d6f, "Reprisal"},
  {0x1d72, "Interject"},
  {0x1d74, "Low Blow"},
  {0x1d71, "Shirk"},
  {0x1d7c, "Arm's Length"},

  // Being GLA, PLD skills
  {0x0009, "Fast Blade"},
  {0x000f, "Riot Blade"},
  {0x0014, "Fight or Flight"},
  {0x1cd5, "Total Eclipse"},
  {0x0010, "Shield Bash"},
  {0x0011, "Sentinel"},
  {0x0017, "Circle of Scorn"},
  {0x001d, "Spirits Within"},
  {0x001c, "Iron Will"},
  {0x0018, "Shield Lob"},
  {0x001e, "Hallowed Ground"},
  
  // PLD only skills
  {0x0dd2, "Goring Blade"},
  {0x0dd3, "Royal Authority"},
  {0x0dd4, "Divine Veil"},
  {0x0dd5, "Clemency"},
  {0x0dd6, "Shelltron"},
  {0x1cd6, "Intervention"},
  {0x1cd7, "Requiescat"},
  {0x1cd8, "Holy Spirit"},
  {0x1cd9, "Passage of Arms"},

  {0x404a, "Holy Circle"},
  {0x404b, "Confiteor"},
  {0x404c, "Atonement"},
  {0x404d, "Intervene"},
  {0x4049, "Prominence"},

  // Begin MRD, WAR skills
  {0x001f, "Heavy Swing"},
  {0x0025, "Maim"},
  {0x0029, "Overpower"},
  {0x0030, "Defiance"},
  {0x002e, "Tomahawk"},
  {0x002a, "Storm's Path"},
  {0x0028, "Thrill of Battle"},
  {0x002c, "Vengeance"},
  {0x002b, "Holmgang"},
  {0x002d, "Storm's Eye"},
  {0x0033, "Steel Cyclone"},
  //{0x0038, "Berserk"}, code is incorrect

  // Begin WAR only skills
  {0x0034, "Infuriate"},
  {0x1cda, "Onslaught"},
  {0x1cdb, "Upheaval"},
  {0x1cdc, "Shake It Off"},
  {0x1cdd, "Inner Release"},
  {0x404e, "Mythril Tempest"},
  {0x404f, "Chaotic Cyclone"},
  {0x4050, "Nascent Flash"},
  {0x4051, "Inner Chaos"},
  {0x0ddd, "Fell Cleave"},
  {0x0dde, "Decimate"},
  {0x0ddf, "Raw Intuition"},
  {0xde0, "Equilibrium"},



  // Begin GNB skills
  {0x3f09, "Keen Edge"},
  {0x3f0a, "No Mercy"},
  {0x3f0b, "Brutal Shell"},
  {0x3f0c, "Camouflage"},
  {0x3f0d, "Demon Slice"},
  {0x3f0e, "Royal Guard"},
  {0x3f0f, "Lightning Shot"},
  {0x3f10, "Danger Zone"},
  {0x3f11, "Solid Barrel"},
  {0x3f12, "Gnashing Fang"},
  {0x3f13, "Savage Claw"},
  {0x3f14, "Nebula"},
  {0x3f15, "Demon Slaughter"},
  {0x3f16, "Wicked Talon"},
  {0x3f17, "Aurora"},
  {0x3f18, "Superbolide"},
  {0x3f19, "Sonic Break"},
  {0x3f1a, "Rough Divide"},
  {0x3f1c, "Jugular Rip"},
  {0x3f1b, "Continuation"},
  {0x3f1d, "Abdomen Tear"},
  {0x3f1e, "Eye Gouge"},
  {0x3f1f, "Bow Shock"},
  {0x3f20, "Heart of Light"},
  {0x3f21, "Heart of Stone"},
  {0x3f22, "Burst Strike"},
  {0x3f23, "Fated Circle"},
  {0x3f24, "Bloodfest"},
  {0x3f25, "Blasting Zone"},


  // Begin DRK skills
  {0x0e21, "Hard Slash"},
  {0x0e25, "Unleash"},
  {0x0e27, "Syphon Strike"},
  {0x0e28, "Unmend"},
  {0x0e29, "Blood Weapon"},
  {0x0e2d, "Grit"},
  {0x0e30, "Souleater"},
  {0x0e32, "Dark Mind"},
  {0x0e34, "Shadow Wall"},
  {0x0e36, "Living Dead"},
  {0x0e38, "Plunge"},
  {0x0e39, "Abyssal Drain"},
  {0x0e3b, "Carve and Split"},
  {0x1cde, "Delirium"},
  {0x1cdf, "Quietus"},
  {0x1ce0, "Bloodspiller"},
  {0x1ce1, "The Blackest Night"},
  {0x4054, "Stalwart Soul"},
  {0x4055, "Flood of Shadow"},
  {0x4056, "Edge of Shadow"},
  {0x4057, "Dark Missionary"},
  {0x4058, "Living Shadow"},

  /** END TANK SECTION **/

  /** BEGIN HEALER SECTION **/

  // Begin Healer Role Skills
  {0x40b0, "Repose"},
  {0x1d87, "Surecast"},
  {0x1d89, "Swiftcast"},
  {0x1d8a, "Lucid Dreaming"},
  {0x1d90, "Esuna"},




  // Begin WHM Skills
  // Note: due to so many spell upgrades,
  // lower level spells are still not here
  {0x0078, "Cure"},
  {0x007c, "Medica"},
  {0x0083, "Cure III"},
  {0x0085, "Medica II"},
  {0x0086, "Fluid Aura"},
  {0x0087, "Cure II"},
  {0x0088, "Presence of Mind"},
  {0x0089, "Regen"},
  {0x008b, "Holy"},
  {0x008c, "Benediction"},
  {0x0df2, "Tetragrammaton"},
  {0x0df3, "Assize"},
  {0x1d06, "Thin Air"},
  {0x1d08, "Divine Benison"},
  {0x1d09, "Plenary Indulgence"},
  {0x4093, "Afflatus Solace"},
  {0x4094, "Dia"},
  {0x4095, "Glare"},
  {0x4096, "Afflatus Rapture"},
  {0x4097, "Afflatus Misery"},
  {0x4098, "Temperance"},

  // Begin SCH Skills
  // Note: due to so many spell upgrades
  // lower levelSpells are still not here
  {0x00a6, "Aetherflow"},
  {0x00a7, "Energy Drain"},
  {0x00b9, "Adloquium"},
  {0x00ba, "Succor"},
  {0x00bd, "Lustrate"},
  {0x00be, "Physick"},
  {0x0dff, "Indomitability"},
  {0x0e01, "Deployment Tactics"},
  {0x0e02, "Emergency Tactics"},
  {0x0e03, "Dissipation"},
  {0x0e1d, "Collective Unconscious"},
  {0x1d0a, "Excogitation"},
  {0x1d0c, "Chain Stratagem"},
  {0x1d0d, "Aetherpact"},
  {0x1ebd, "Aetherpact End"},
  {0x4099, "Whispering Dawn"},
  {0x409a, "Fey Illumination"},
  {0x409b, "Art of War"},
  {0x409c, "Biolysis"},
  {0x409d, "Broil III"},
  {0x409e, "Recitation"},
  {0x409f, "Fey Blessing"},
  {0x40a1, "Summon Seraph"},
  {0x40a2, "Consolation"},
  {0x433f, "Summon Eos"},
  {0x4340, "Summon Selene"},
  {0x45ce, "Ruin II"},

  // Begin AST skills
  // TODO: Certain skills are still missing due to
  //  spell upgrades or needing a target player
  {0x40ab, "Malefic IV"},
  {0x0e0a, "Benefic"},
  {0x0e16, "Lightspeed"},
  {0x40aa, "Combust III"},
  {0x0e10, "Helios"},
  {0x0e1e, "Essential Dignity"},
  {0x0e1a, "Benefic II"},
  {0x0e06, "Draw"},
  {0x0e14, "Diurnal Sect"},
  {0x259d, "Undraw"},
  {0x0e0b, "Aspected Benefic"},
  {0x0e11, "Aspected Helios"},
  {0x0e1f, "Gravity"},
  {0x0e15, "Nocturnal Sect"},
  {0x40ac, "Celestial Intersection"},
  {0x40ad, "Horoscope"},
  {0x40a9, "Celestial Opposition"},
  {0x1d18, "Sleeve Draw"},
  {0x1d14, "Lord of Crowns"},
  {0x40af, "Neutral Sect"},
  {0x1131, "The Balance"},
  {0x1132, "The Arrow"},
  {0x1133, "The Spear"},
  {0x1134, "The Bole"},
  {0x1135, "The Ewer"},
  {0x1136, "The Spire"},
  {0x1d15, "Lady of Crowns"},
  {0x40a8, "Divination"},
  {0x0e09, "Redraw"},
  {0x42ff, "Aspected Benefic"}, // Nocturnal
  {0x4300, "Aspected Helios"}, // Nocturnal

  /** END HEALER SECTION **/

  /** BEGIN MELEE DPS SECTION **/

  // Begin melee DPS role actions
  // Note: Arm's Length is already handled with tank role actions.
  {0x1d75, "Second Wind"},
  {0x1d76, "Bloodbath"},
  {0x1d7a, "True North"},
  {0x1d7d, "Feint"},
  {0x1eb7, "Leg Sweep"},

  // Begin PUG, MNK skills
  {0x0035, "Bootshine"},
  {0x0036, "True Strike"},
  {0x0038, "Snap Punch"},
  {0x003c, "Fists of Earth"},
  {0x003d, "Twin Snakes"},
  {0x003e, "Arm of the Destroyer"},
  {0x0041, "Mantra"},
  {0x0042, "Demolish"},
  {0x0045, "Perfect Balance"},
  {0x0049, "Fists of Wind"},

  // Begin MNK skills
  {0x003f, "Fists of Fire"},
  {0x0046, "Rockbreaker"},
  {0x0047, "Shoulder Tackle"},
  {0x004a, "Dragon Kick"},
  {0x0dd7, "Tornado Kick"},
  {0x0dd9, "Elixir Field"},
  {0x0dda, "Meditation"},
  {0x0ddb, "The Forbidden Chakra"},
  {0x10a6, "Form Shift"},
  {0x1ce2, "Riddle of Earth"},
  {0x1ce3, "Riddle of Fire"},
  {0x1ce4, "Brotherhood"},
  {0x4059, "Four-point Fury"},
  {0x405a, "Enlightenment"},
  {0x405b, "Anatman"},
  {0x405c, "Six-sided Star"},


  /** END MELEE DPS SECTION **/

  /** BEGIN PHYSICAL RANGED SECTION **/

  // Begin physical DPS role actions


  /** END PHYSICAL RANGED SECTION **/

  /** BEGIN MAGIC RANGED SECTION **/

  // Begin Magic DPS Role Actions
  // Most of these are shared with healers
  // Only Addle is not
  {0x1d88, "Addle"},

  // Begin RDM skills
  {0x1d50, "Riposte"},
  {0x1d51, "Verthunder"},
  {0x1d52, "Corps-a-corps"},
  {0x1d53, "Veraero"},
  {0x1d56, "Verfire"},
  {0x1d57, "Verstone"},
  {0x1d58, "Zwercchau"},
  {0x1d59, "Moulinet"},
  {0x1d5a, "Vercure"},
  {0x1d5b, "Displacement"},
  {0x1d5c, "Redoublement"},
  {0x1d5d, "Fleche"},
  {0x1d5e, "Acceleration"},
  {0x1d5f, "Contre Sixte"},
  {0x1d60, "Embolden"},
  {0x1d61, "Manafication"},
  {0x1d64, "Jolt II"},
  {0x1d65, "Verflare"},
  {0x1d66, "Verholy"},
  {0x1d67, "Enchanted Riposte"},
  {0x1d68, "Enchanted Zwercchau"},
  {0x1d69, "Enchanted Redoublement"},
  {0x1d6a, "Enchanted Moulinet"},
  {0x408c, "Verthunder II"},
  {0x408d, "Veraero II"},
  {0x408e, "Impact"},
  {0x408f, "Engagement"},
  {0x4090, "Enchanted Reprise"},
  {0x4091, "Reprise"},
  {0x4092, "Scorch"},


  /** END MAGIC RANGED SECTION **/
};

#endif
