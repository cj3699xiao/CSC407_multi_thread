/*--------------------------------------------------------------------------*
 *----									----*
 *----		joey.c							----*
 *----									----*
 *----	    This program implements the joey processes, which try to 	----*
 *----	leave the mall.							----*
 *----									----*
 *----	----	----	----	----	----	----	----	----	----*
 *----									----*
 *----	Version 1.0		2017 April 17		Joseph Phillips	----*
 *----									----*
 *--------------------------------------------------------------------------*/

//
//		Header inclusion:
//
#include	"kangarooHeaders.h"


//
//		Definition of global vars:
//

//  PURPOSE:  To tell if the mama is still looking for joeys.
int		shouldStillRun		= 1;

//  PURPOSE:  To tell which joey this joey is.
int		joeyIndex;

//  PURPOSE:  To tell the process id of the mall process.
pid_t		mallPid;

//  PURPOSE:  To hold what the next turn will be.
int		nextTurn;

//  PURPOSE:  To initialize most globals given the 'argc' command line
//	arguments pointed to by 'argv[]'.  No return value.
void		initializeMostGlobals	(int		argc,
					 char*		argv[]
					)
{
  //  I.  Application validity check:
  if  (argc < MIN_JOEY_CMD_LINE_PARAMS)
  {
    fprintf(stderr,"Usage: %s <index> <mallPid> <randSeed>\n",JOEY_PROG_NAME);
    exit(EXIT_FAILURE);
  }

  //  II.  Initialize globals:
  //  II.A.  Initialize 'joeyIndex':
  joeyIndex	= strtol(argv[JOEYS_INDEX_CMD_LINE_INDEX],NULL,10); // <-- REPLACE with code that gets integer from 'argv[JOEYS_INDEX_CMD_LINE_INDEX]'

  //  II.B.  Initialize 'mallPid':
  mallPid	= strtol(argv[JOEYS_MALL_PID_CMD_LINE_INDEX],NULL,10); // <-- REPLACE with code that gets integer from 'argv[JOEYS_MALL_PID_CMD_LINE_INDEX]'

  //  II.C.  Initialize random number generator:
  int	seed	= strtol(argv[JOEYS_RAND_SEED_CMD_LINE_INDEX],NULL,10); // <-- REPLACE with code that gets integer from 'argv[JOEYS_RAND_SEED_CMD_LINE_INDEX]'

  srand(seed);

  //  III.  Finished:
}


//  PURPOSE:  To handle 'CORRECT_TURN_SIGNAL'.  Ignores 'sig'.  No return value.
void		correctTurnSignalHandler	(int		sig
						)
{
  //  I.  Application validity check:

  //  II.  
  nextTurn = ((rand() % 2) == 1) ? LEFT_TURN_SIGNAL : RIGHT_TURN_SIGNAL;
  printf("Joey # %d: Yay!  On to the next turn; I'll guess %s.\n",
  	  joeyIndex,
	  (nextTurn == LEFT_TURN_SIGNAL) ? "left" : "right"
	);
  fflush(stdout);
  // YOUR CODE HERE to send 'nextTurn' to 'mallPid'

  kill(mallPid,nextTurn);

  //  III.  Finished:
}


//  PURPOSE:  To handle 'WRONG_TURN_SIGNAL'.  Ignores 'sig'.  No return value.
void		wrongTurnSignalHandler		(int	      sig
						)
{
  //  I.  Application validity check:

  //  II.  
  nextTurn = (nextTurn == LEFT_TURN_SIGNAL)
  	     ? RIGHT_TURN_SIGNAL
	     : LEFT_TURN_SIGNAL;
  printf("Joey # %d: Oops!  Let us try the other direction: %s.\n",
  	  joeyIndex,
	  (nextTurn == LEFT_TURN_SIGNAL) ? "left" : "right"
	);
  fflush(stdout);
  // YOUR CODE HERE to send 'nextTurn' to 'mallPid'
  kill(mallPid,nextTurn);

  //  III.  Finished:
}


//  PURPOSE:  To note that the program may now finish (after receiving
//	'SIGINT').  Ignores 'sig'.   No return value.
void		sigIntHandler			(int		sig
						)
{
  printf("Joey # %d: Yay!  I'm safely in mommy's pouch!\n",joeyIndex);
  fflush(stdout);
  shouldStillRun	= 0;
}


//  PURPOSE:  To install 'correctTurnSignalHandler()' as the signal handler for
//	'CORRECT_TURN_SIGNAL' and 'wrongTurnSignalHandler()' as the handler for
//	'WRONG_TURN_SIGNAL'.  No parameters.  No return value.
void		installTurnHandlers		()
{
  //  I.  Application validity check:

  //  II.  Install the handler:
  //  YOUR CODE HERE
  struct sigaction action_true;
  memset(&action_true,'\0',sizeof(action_true));
  sigemptyset(&action_true.sa_mask);
  action_true.sa_handler = correctTurnSignalHandler;
  sigaction(CORRECT_TURN_SIGNAL,&action_true,NULL);

  struct sigaction action_false;
  memset(&action_false,'\0',sizeof(action_false));
  sigemptyset(&action_false.sa_mask);
  action_false.sa_handler = wrongTurnSignalHandler;
  sigaction(WRONG_TURN_SIGNAL,&action_false,NULL);


  //  III.  Finished:  
}


//  PURPOSE:  To install 'sigIntHandler()' as the handler for 'SIGINT'.  No
//	parameters.  No return value.
void		installSigIntHandler		()
{
  //  I.  Application validity check:

  //  II.  Install handler:
  //  II.A.  Set up struct to specify the new action.
  //  YOUR CODE HERE
  struct sigaction action_int;
  memset(&action_int,'\0',sizeof(action_int));
  sigemptyset(&action_int.sa_mask);
  action_int.sa_handler = sigIntHandler;
  sigaction(SIGINT,&action_int,NULL);
  
  

  //  III.  Finished:
}


int	main			(int		argc,
				 char*		argv[]
				)
{
  //  I.  Application validity check (done by 'initializeMostGlobals()'):

  //  II.  Try to leave mall:
  //  II.A.  Initialize globals:
  initializeMostGlobals(argc,argv);
  installTurnHandlers();
  installSigIntHandler();

  //  II.B.  Try to leave mall:
  nextTurn = ((rand() % 2) == 1) ? LEFT_TURN_SIGNAL : RIGHT_TURN_SIGNAL;
  printf("Joey # %d: Which direction?  Decisions, Decisions!  I'll guess %s.\n",
  	  joeyIndex,
	  (nextTurn == LEFT_TURN_SIGNAL) ? "left" : "right"
	);
  fflush(stdout);
  // YOUR CODE HERE to send 'nextTurn' to 'mallPid'
  kill(mallPid,nextTurn);
  while  (shouldStillRun)
    sleep(1);

  //  III.  Finished:
  return(EXIT_SUCCESS);
}
