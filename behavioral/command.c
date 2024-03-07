#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct _receiver
{
  void (*action)(const char* s);
} receiver_t;

#define COMMAND_MEMBERS(TYPE)                                                                                          \
  void (*Command)(TYPE);                                                                                               \
  void (*Execuete)(TYPE);

typedef struct _command_t
{
  COMMAND_MEMBERS(struct _command_t*);
  receiver_t* receiver;
} command_t;

typedef struct _concreteCommand_t
{
  COMMAND_MEMBERS(struct _concreteCommand_t*);
  receiver_t* receiver;
  int val;
} concreteCommand_t;

typedef struct _invoke_t
{
  void (*executeCommand)(struct _invoke_t* invoke);
  void (*appendCommand)(struct _invoke_t* invoke, command_t* command);
  command_t* commands[10];
  int count;
} invoke_t;

void receiverAction(const char* s)
{
  printf("Receiver action: %s", s);
}

void invokeExecuteCommand(invoke_t* invoke)
{
  for (int i = 0; i < invoke->count; i++)
  {
    invoke->commands[i]->Execuete(invoke->commands[i]);
  }
}

void invokeSetCommand(invoke_t* invoke, command_t* command)
{
  if (invoke->count >= 10)
    return;
  invoke->commands[invoke->count++] = command;
}

void concreteCommandCommand1(concreteCommand_t* concrete_command)
{
  concrete_command->receiver->action("COMMAND1\n");
  concrete_command->val++;
}

void concreteCommandCommand2(concreteCommand_t* concrete_command)
{
  concrete_command->receiver->action("COMMAND2\n");
  concrete_command->val++;
}

void concreteCommandExecution1(concreteCommand_t* concrete_command)
{
  concrete_command->Command(concrete_command);
  printf("Run concrete command 1, %d\n", concrete_command->val);
}

void concreteCommandExecution2(concreteCommand_t* concrete_command)
{
  concrete_command->Command(concrete_command);
  printf("Run concrete command 2, %d\n", concrete_command->val);
}

int main()
{
  receiver_t receiver = { .action = receiverAction };
  concreteCommand_t concrete_command1 = {
    .Execuete = concreteCommandExecution1, .receiver = &receiver, .val = 123, .Command = concreteCommandCommand1
  };
  concreteCommand_t concrete_command2 = {
    .Execuete = concreteCommandExecution2, .receiver = &receiver, .val = 789, .Command = concreteCommandCommand2
  };

  invoke_t invoke = {
    .commands = { NULL }, .count = 0, .executeCommand = invokeExecuteCommand, .appendCommand = invokeSetCommand
  };

  invoke.appendCommand(&invoke, (command_t *)&concrete_command1);
  invoke.appendCommand(&invoke, (command_t *)&concrete_command2);

  printf("Run invoke function five times\n");
  for (int i = 0; i < 5; i++)
  {
    invoke.executeCommand(&invoke);
  }

  return 0;
}
