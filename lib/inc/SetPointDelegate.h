/*
 * Copyright 2017 BrewPi/Elco Jacobs.
 *
 * This file is part of BrewPi.
 * 
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <stdint.h>
#include "ControllerMixins.h"
#include "Interface.h"

class SetPointDelegate :
    public SetPoint,
    public SetPointDelegateMixin
{
public:
    SetPointDelegate(BaseLookup const& lookup) : delegate(lookup){}
    ~SetPointDelegate() = default;

    virtual void accept(VisitorBase & v) override final {
        v.visit(*this);
    }

    virtual temp_t read() const override final {
        return delegate().read();
    }

    virtual void write(temp_t val) override final {
        delegate().write(val);
    }

private:
    RefTo<SetPoint> delegate;

friend class SetPointDelegateMixin;
};