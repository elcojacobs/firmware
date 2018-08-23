/*
 * Copyright 2014-2015 Matthew McGowan.
 * Copyright 2018 BrewBlox / Elco Jacobs
 * This file is part of Controlbox.
 *
 * Controlbox is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Controlbox.  If not, see <http://www.gnu.org/licenses/>.
 */


#pragma once

#include "Object.h"
#include "Container.h"
#include "Connections.h"
#include "DataStream.h"
#include "DataStreamConverters.h"
#include "EepromObjectStorage.h"
#include "CboxError.h"
#include "ObjectFactory.h"
#include <memory>

namespace cbox {

class Box
{
private:
	// A single container is used for both system and user objects.
	// The application can add the system objects first, then set the start ID to a higher value.
	// The objects with an ID lower than the start ID cannot be deleted.
    ObjectFactory& factory;
    ObjectContainer& objects;
	ObjectStorage& storage;
	// Box receives commands from connections in the connection pool and streams back the answer to the same connection
	ConnectionPool& connections;
	uint8_t activeProfiles;

	// command handlers
	void noop(CrcDataIn& in, DataOut& out);
	void invalidCommand(CrcDataIn& in, DataOut& out);
	void readObject(CrcDataIn& in, DataOut& out);
	void writeObject(CrcDataIn& in, DataOut& out);
	void createObject(CrcDataIn& in, DataOut& out);
	void deleteObject(CrcDataIn& in, DataOut& out);
	void listActiveObjects(CrcDataIn& in, DataOut& out);
	void listSavedObjects(CrcDataIn& in, DataOut& out);
	void clearObjects(CrcDataIn& in, DataOut& out);
	void reboot(CrcDataIn& in, DataOut& out);
	void factoryReset(CrcDataIn& in, DataOut& out);

	// addContainedObjectFromStream is available with 2 prototypes. With and without CRC checked input
	// this is because EEPROM data does not have a CRC
	CboxError addContainedObjectFromStream(CrcDataIn& in, obj_id_t & id, bool replace);
	CboxError addContainedObjectFromStream(DataIn& in, obj_id_t & id, bool replace);
	CboxError addContainedObjectFromStreamImpl(DataIn& in, obj_id_t & id, uint8_t & profiles, std::unique_ptr<Object> &newObj);

	void loadObjectsFromStorage();

public:
	Box(ObjectFactory& _factory, ObjectContainer& _objects, ObjectStorage& _storage, ConnectionPool & _connections);
	~Box() = default;

	void handleCommand(DataIn& data, DataOut& out);

	// process all incoming messages assuming they are hex encoded
    void hexCommunicate();

    auto getObject(const obj_id_t& id){
        return objects.fetch(id);
    }

	void setActiveProfilesAndUpdateObjects(uint8_t newProfiles);

	uint8_t getActiveProfiles() const {
		return activeProfiles;
	}

	inline const obj_id_t userStartId() const {
	    return obj_id_t(100);
	}

	enum CommandID : uint8_t {
		NONE = 0,				// no-op
	   	READ_OBJECT = 1,		// stream an object to the data out
	   	WRITE_OBJECT = 2,		// stream new data into an object from the data in
	   	CREATE_OBJECT = 3,		// add a new object
		DELETE_OBJECT = 4,		// delete an object by id
		LIST_ACTIVE_OBJECTS = 5,// list objects saved to persistent storage
		LIST_STORED_OBJECTS = 6,// list objects saved to persistent storage
		CLEAR_OBJECTS = 7,      // remove all user objects
		REBOOT = 8,				// reboot the system
		FACTORY_RESET = 9,		// erase all settings and reboot
	};
};

// the ProfilesObject can added to a box, so the active profile can be written as a system object and is also persisted
class ProfilesObject: public Object
{
    Box * myBox;

public:
    ProfilesObject(Box * box) : myBox(box){}

    virtual cbox::CboxError streamFrom(cbox::DataIn& dataIn) override final {
        uint8_t newProfiles;
        if(!dataIn.get(newProfiles)){
            return CboxError::input_stream_read_error;
        }
        myBox->setActiveProfilesAndUpdateObjects(newProfiles);
        return CboxError::no_error;
    }

    virtual cbox::CboxError streamTo(cbox::DataOut& out) const override final {
        uint8_t profiles = myBox->getActiveProfiles();
        if(!out.put(profiles)){
            return CboxError::output_stream_write_error;
        }
        return CboxError::no_error;
    }

    virtual cbox::obj_type_t typeId() const override final {
        return resolveTypeId(this);
    }
};

} // end namespace cbox
