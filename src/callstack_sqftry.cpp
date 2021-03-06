#include "callstack_sqftry.h"
#include "value.h"
#include "virtualmachine.h"
#include "codedata.h"

std::shared_ptr<sqf::instruction> sqf::callstack_sqftry::popinst(sqf::virtualmachine * vm)
{
	if (mdoExcept)
	{
		clear_stack();
		auto sptr = std::shared_ptr<callstack_sqftry>(this, [](callstack_sqftry*) {});
		mcatchblock->loadinto(vm->stack(), sptr);
		setvar("_exception", std::make_shared<value>(mmessage));
		mdoExcept = false;
	}
	return callstack::popinst(vm);
}
