#include "../commandmap.h"
#include "../value.h"
#include "../cmd.h"
#include "../virtualmachine.h"
#include "../sidedata.h"
#include "../groupdata.h"
#include "../objectdata.h"
#include "../innerobj.h"
#include "../arraydata.h"
#include <sstream>

using namespace sqf;
namespace
{
	std::shared_ptr<value> grpnull_(virtualmachine* vm)
	{
		return std::make_shared<value>(groupdata::create(), type::GROUP);
	}
	std::shared_ptr<value> creategroup_side(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto side = right->data<sidedata>();
		return std::make_shared<value>(groupdata::create(vm, side), type::GROUP);
	}
	std::shared_ptr<value> groupid_group(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto grp = right->data<groupdata>();
		return std::make_shared<value>(grp->groupid());
	}
	std::shared_ptr<value> units_group(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto grp = right->data<groupdata>();
		auto arr = std::make_shared<arraydata>();
		for (auto& unit : grp->get_units())
		{
			arr->push_back(std::make_shared<value>(std::make_shared<objectdata>(unit), OBJECT));
		}
		return std::make_shared<value>(arr, ARRAY);
	}
	std::shared_ptr<value> units_object(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto grp = right->data<objectdata>()->obj()->group();
		auto arr = std::make_shared<arraydata>();
		if (grp)
			for (auto& unit : grp->get_units())
			{
				arr->push_back(std::make_shared<value>(std::make_shared<objectdata>(unit), OBJECT));
			}
		return std::make_shared<value>(arr, ARRAY);
	}
	std::shared_ptr<value> deletegroup_group(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto grp = right->data<groupdata>();
		if (grp->is_empty())
		{
			vm->drop_group(grp);
		}
		else
		{
			vm->wrn() << "Attempt to delete a non-empty group was made. GroupID: " << grp->groupid() << '.' << std::endl;
		}
		return std::make_shared<value>();
	}
	std::shared_ptr<value> isnull_group(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto grp = right->data<groupdata>();
		return std::make_shared<value>(grp->is_null());
	}
	std::shared_ptr<value> side_group(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto grp = right->data<groupdata>();
		return std::make_shared<value>(grp->side(), SIDE);
	}
}
void sqf::commandmap::initgroupcmds()
{
	//GetVariable & SetVariable & AllVariables are in namespacecmds as simple alias.
	add(nular("blufor", "Western side.", [](virtualmachine* vm) -> std::shared_ptr<value> { return std::make_shared<value>(std::make_shared<sidedata>(sidedata::West), type::SIDE); }));
	add(nular("west", "Western side.", [](virtualmachine* vm) -> std::shared_ptr<value> { return std::make_shared<value>(std::make_shared<sidedata>(sidedata::West), type::SIDE); }));
	add(nular("opfor", "Eastern side.", [](virtualmachine* vm) -> std::shared_ptr<value> { return std::make_shared<value>(std::make_shared<sidedata>(sidedata::East), type::SIDE); }));
	add(nular("east", "Eastern side.", [](virtualmachine* vm) -> std::shared_ptr<value> { return std::make_shared<value>(std::make_shared<sidedata>(sidedata::East), type::SIDE); }));
	add(nular("resistance", "Guerilla side.", [](virtualmachine* vm) -> std::shared_ptr<value> { return std::make_shared<value>(std::make_shared<sidedata>(sidedata::Guerilla), type::SIDE); }));
	add(nular("independent", "Guerilla side.", [](virtualmachine* vm) -> std::shared_ptr<value> { return std::make_shared<value>(std::make_shared<sidedata>(sidedata::Guerilla), type::SIDE); }));
	add(nular("civilian", "Civilian side.", [](virtualmachine* vm) -> std::shared_ptr<value> { return std::make_shared<value>(std::make_shared<sidedata>(sidedata::Civilian), type::SIDE); }));
	add(nular("sideEmpty", "Empty side.", [](virtualmachine* vm) -> std::shared_ptr<value> { return std::make_shared<value>(std::make_shared<sidedata>(sidedata::Empty), type::SIDE); }));
	add(nular("sideEnemy", "Enemy side.", [](virtualmachine* vm) -> std::shared_ptr<value> { return std::make_shared<value>(std::make_shared<sidedata>(sidedata::Enemy), type::SIDE); }));
	add(nular("sideFriendly", "Friendly side.", [](virtualmachine* vm) -> std::shared_ptr<value> { return std::make_shared<value>(std::make_shared<sidedata>(sidedata::Friendly), type::SIDE); }));
	add(nular("sideLogic", "Logic side.", [](virtualmachine* vm) -> std::shared_ptr<value> { return std::make_shared<value>(std::make_shared<sidedata>(sidedata::Logic), type::SIDE); }));
	add(nular("sideUnknown", "Unknown side.", [](virtualmachine* vm) -> std::shared_ptr<value> { return std::make_shared<value>(std::make_shared<sidedata>(sidedata::Unknown), type::SIDE); }));

	add(nular("grpNull", "A non-existing Group. To compare non-existent groups use isNull or isEqualTo.", grpnull_));
	add(unary("createGroup", type::SIDE, "Creates a new Group for the given Side.", creategroup_side));
	add(unary("groupId", type::GROUP, "Returns group name.", groupid_group));
	add(unary("units", type::GROUP, "Returns an array with all the units in the group.", units_group));
	add(unary("units", type::OBJECT, "Returns an array with all the units in the group of the unit. For a destroyed object an empty array is returned.", units_object));
	add(unary("deleteGroup", type::GROUP, "Destroys the given group. Group must be empty.", deletegroup_group));
	add(unary("isNull", type::GROUP, "Checks whether the tested item is Null.", isnull_group));
	add(unary("side", type::GROUP, "Returns the side of a group.", side_group));
}
