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

	enum Packets {
		ServerInfo, // string game version, string hostname, string description, byte curplayers, byte max players, arr_mods[string name, string version]
		            // before joining a server, it might be nice to show a info dialog., connect to server, send this packet, kill connection.

		Login, // string name, arr_mods[string name, string version]
			   // server responds with a kick, or the asigned PID, and EID, once you've recieved the first EntityTeleport for your asigned EID, you're fully joined.

		Ping, // long timestamp, this gets sent by the server, client needs to respond with the timestamp that the server sends (latency check)
		ChatMessage, // PID, string msg. Client only sends msg.

		// everything here is server sent only, unless stated.
		EntityCreate, // EID, string type, property_object
		EntityRemove, // EID
		EntityProperty, // EID, property_object
		EntityMove, // EID, mx, my, mz
		EntityTeleport, // EID, x,y,z, p,y,r
		EntityAngle, // EID, p,y,r

		PlayerDig, // PID, x,y,z, byte side, client can send this whitout the PID
		PlayerUse, // PID, x,y,z, byte side, client can send this whitout the PID
		PlayerJoin, // PID, string name
		PlayerLeave, // PID, client should send this when he disconnects, it's a better message than "END_OF_STREAM"
		PlayerKey, // PID, byte key [wasd], bool pressing [true/false], client can send this whitout the PID
		PlayerRespawn, // client sends this to the server if he needs to respawn.

		ModMessage, // string modname, ? data. client can also send this

		ChangeWorld, // string name. When changing world, unload ALL chunks and entities. the server wont inform you as everything's new anyhow.
		             // this also gets sent at Login, as you're entering a new world.

		ChunkLoad, // X, Y, int zlib_buffer_len, byte[] zlib_buffer
		ChunkUnload, // X, Y

		BlockChange, // WX, WY, Z, ushort block
		BlockMeta, // WX, WY, Z, byte meta
		BlockMultiChange, // start WX, start WY, start z, width, dept, height, per block[ushort block, byte meta]


		Kick = 0xFF // string reason
	};

	/*
		How one connects:
		> connect to server
		> send login
		< receive kick if you're a dickhead
		< receive login packet with PID and EID
		< receive entity list
		< receive your X,Y,Z, P,Y,R
		< receive 16x16 chunks
		. and you start running around and doing shit!
	*/
}
