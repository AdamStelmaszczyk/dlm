all: test_case lock_test 

test_case: test_case.cpp ../api/dlm.cpp ../exceptions/Warning.cc ../exceptions/Error.cc ../lock_manager/LockManager.cc ../lock_manager/Lock.cc ../lock_manager/LockOwner.cc ../lock_manager/WaitingLock.cc ../ui/SimpleConsole.cc ../config/Config.cc ../listener/Cleaner.cc ../logger/Logger.cc ../listener/Listener.cc
	g++ test_case.cpp ../api/dlm.cpp  ../exceptions/Warning.cc ../exceptions/Error.cc ../lock_manager/LockManager.cc  ../lock_manager/Lock.cc ../lock_manager/LockOwner.cc ../lock_manager/WaitingLock.cc ../ui/SimpleConsole.cc ../config/Config.cc ../listener/Cleaner.cc ../logger/Logger.cc ../listener/Listener.cc -lpthread -o test_case

lock_test: ../api/dlm.cpp lock_test.cpp
	g++ ../api/dlm.cpp lock_test.cpp -o lock_test
