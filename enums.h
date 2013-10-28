
namespace Biomes {
	enum Enum {
		Winter, Casual, Swamp, Desert, Mountains, Vulcano
	};
}

namespace Blocks {
	enum Enum {
		Air, // always zero, it is sad asian.

		// Building blocks
		Stone, Grass, Dirt, Bedrock, Sand, SandStone, Gravel, Planks, Wood, Clay, Obsidian, Wool, BookShelf, CobbleStone, MossyCobblestone,
		IronBlock, GoldBlock, DiamondBlock, Glass,

		// Misc blocks
		Leaves, Water, Lava, Ice, Snow,

		// Lightning
		Torch, RedstoneTorch,

		// Useables
		CraftingTable, Chest, Furnace, 

		// Green
		Cactus, FlowerRed, FlowerYellow, DeadBush, TallGrass, SugarCane, Sapling,

		// Ores
		GoldOre, IronOre, CoalOre, DiamondOre, LazuliOre, RedstoneOre, 

		// Fun and annoying
		Rail, TNT, Spawner, Door, Door_TOP, Redstone, NoteBlock, Piston, StickyPiston, PresurePlate, Jukebox, Lever, Button, Dispenser, Dropper, Hopper,
		Tube, PowerCable, SolarPanel, BatBox, Fence, Sign,
		
		BLOCKS_END // mark end index to resume at items
	};
}

namespace Items {
	enum Enum {		
		// -- Items --
		// Even more fun and annoying!
		Minecart = Blocks::BLOCKS_END, Igniter, Bow, Bucket,

		// Food
		Apple, RawBeef, RawChicken, CookedBeef, CookedChicken, Cake,

		// crafting
		Coal, Iron, Gold, Diamond, Stick, Battery, 

		// tools
		Pickaxe_Wood = 0xFF, Pickaxe_Stone, Pickaxe_Iron, Pickaxe_Gold, Pickaxe_Diamond,
		Axe_Wood, Axe_Stone, Axe_Iron, Axe_Gold, Axe_Diamond,
		Shovel_Wood, Shovel_Stone, Shovel_Iron, Shovel_Gold, Shovel_Diamond,
		Sword_Wood, Sword_Stone, Sword_Iron, Sword_Gold, Sword_Diamond,
		Sheers,

		// armor
		Armor_Iron_Feet, Armor_Iron_Legs, Armor_Iron_Body, Armor_Iron_Head,
		Armor_Gold_Feet, Armor_Gold_Legs, Armor_Gold_Body, Armor_Gold_Head,
		Armor_Diamond_Feet, Armor_Diamond_Legs, Armor_Diamond_Body, Armor_Diamond_Head,
	};
}

namespace PlayerWindow {
	enum Enum {
		Inventory, Chest, CraftingGrid
	};
}

namespace Packets {
	// PID = playerID, byte
	// EID = EntID, Uint

	// Ent x,y,z and p,y,r are doubles. p,y,r means pitch yaw roll
	// also counts for mx,my,mz and my, mp, mr, the move packet is to smooth out movement, rather than teleporting.

	// Chunk X,Y are shorts
	// Block X, Y are ints, also refered to as WX, WY (world-X/Y). The Z is a byte.

	// arr means array, which is indexed with an int

	// property_object is a string with the name of the property, and then a byte telling you what type it is
	// enum{ Bool, Int, Double, String, Table }
	// if the name of the property_object is "_ROOT_" then this is the complete property table. this mainly gets send on the entity create.

	// if PID is 255, then it's the server

	// item is {short id, byte amount, byte meta}

	// WindowType is an enum, {Inventory, Chest, CraftingGrid}

	enum Packets {
		ServerInfo, // string game version, string hostname, string description, byte curplayers, byte max players, arr_mods[string name, string version]
		            // before joining a server, it might be nice to show a info dialog., connect to server, send this packet, kill connection.
					// this works on TCP and UDP, UDP preferred tho :v


        // everything here is server sent only, unless stated.
        EntityMove, // EID, mx, my, mz, client can send this whitout the EID
        EntityForce, // EID, x,y,z
        EntityAngle, // EID, p,y,r
		EntityCreate, // EID, string type, property_object
		EntityRemove, // EID
        EntityProperty, // EID, property_object
        EntityTeleport, // EID, x,y,z, p,y,r, client can send this whitout the EID
		
		PlayerInventory, // byte slot, item
		PlayerInventoryFull, // 62 times item
							// 0 - 8 slots
							// 9 - 46 main inventory
							// 47 head
							// 48 body
							// 49 legs
							// 50 arms
							// 51 - 59 crafting
							// 60 currently dragging with mouse
							// 61 crafted slot

		PlayerDig, // PID, x,y,z, byte side, client can send this whitout the PID
		PlayerUse, // client only: EID
        PlayerKey, // PID, byte key [wasd], bool pressing [true/false], client can send this whitout the PID
        PlayerHurt, // client only send, EID
        PlayerJoin, // PID, string name
        PlayerPlace, // PID, x,y,z, byte side, client can send this whitout the PID
        PlayerLeave, // PID, client should send this when he disconnects, it's a better message than "END_OF_STREAM"
        PlayerSelect, // PID, byte slot (0 - 8), client can send this whitout the PID
        PlayerRespawn, // client sends this to the server if he needs to respawn.
		PlayerNoclip, // PID, bool isInNoclip, client can send this whitout the PID
		PlayerUseBlock, // client only: x,y,z, byte side
		PlayerMicrophone, // PID, bool istalking, double heardistance, int channels, int sampleRate, int bitsPerSample, client can send this whitout the PID
		PlayerMicrophoneData, // PID, int zlib_buffer_len, byte[] zlib_buffer, client can send this whitout the PID

		PlayerWindowClick,  // byte slotid, bool leftmouse, bool shift
		PlayerCloseWindow,  // if this gets send, close the menu. you should also send this
		PlayerOpenWindow,   // WindowType, block xyz, byte side
							// when aimed at a crafting bench, you send this, the server will respond telling you the same thing, then open it
							// else the server will kick your ass if you do any actions in the window.
							// for the inventory, just send  0,0,0 as blockdata and 0 as side

        Ping, // long timestamp, this gets sent by the server, client needs to respond with the timestamp that the server sends (latency check)
        Login, // CLIENT: string name, arr_mods[string name, string version], SERVER: PID, EID
        ModMessage, // string modname, ? data. client can also send this
        ChatMessage, // PID, string msg. Client only sends msg.

		ChangeWorld, // string name, int vision.
					 // when you get this packet, this means, you're entering a new world, so, delete all chunks, entities, players and stuff.
					 // to know how much chunks you get sent, it's: (vision * 2 + 1) * (vision * 2 + 1)
					 // as example: if vision is 2, you'll receive 25 chunks. at all times.

		ChunkLoad, // X, Y, int zlib_buffer_len, byte[] zlib_buffer, the player can also request a chunk by sending x,y. You'll get kicked if you cannot see it tho.
		ChunkUnload, // X, Y

        BlockMeta, // WX, WY, Z, byte meta
        BlockChange, // WX, WY, Z, ushort block
        BlockMultiChange, // start WX, start WY, start z, width, dept, height, per block[ushort block, byte meta]

        ServerData, 
					// view texture {int w, int h, byte[] zlib_buffer}
					// view texture coordiates byte[] zlib_buffer
					// block texture {int w, int h, byte[] zlib_buffer}
					// block texture coordiates byte[] zlib_buffer
					// block settings byte[] zlib_buffer
					// block boxes byte[] zlib_buffer
					// item recipes byte[] zlib_buffer
					// int count of block textures
					// int count of view textures

					// one setting is a short, with bit flags
					//{
						// 0: contains alpha, for seperate draw calls
						// 1: Ignore collisions
						// 2: Useble block
						// 3: can override, example: water to be ignored if you place a block, so we can "override" it
						// 4: instant DOOM DEATH AND DESTRUCTION HUE HUE HUE HUE
						// 5: wont drop on DOOM DEATH AND DESTRUCTION HUE HUE HUE HUE
						// 6: is sprite, sappling, tallgrass, sugercane, etc.
						// 7: custom collision/render box
						// 8: unused
						// 9 - 13: light level
						// 14 - 15: block destroy type, pickaxe 00, axe 01, shovel 10, sword 11
					//}
					
					// one BLOCK texture coordinate is 6 times 4 floats, StartX, StartY, EndX, EndY
					// one VIEW texture coordinate is 2 times a short, X and Y.
					// one block box is 6 floats, min[x,y,z], max[x,y,z]
					// one recipe is: Short craftID, byte craftCount, 9 shorts ID's needed, 9 bytes counts needed.

        Debug, // for testing shiz
		Kick = 0xFF // string reason
	};

	/*
		How one connects:
		> connect to server
		> send login
		< receive kick if you're a dickhead
		< receive entity list
		< receive login packet with PID and EID
		< receive 16x16 chunks
		# and you start running around and doing shit!
	*/
}