#Specifies the compiler to be used (gcc)
CC=gcc
#Flags to pass to the compiler (here, it includes the directory for header files)
CFLAGS=-I.
#Libraries to link against (here, it specifies the MySQL client library)
LIBS=-lmysqlclient

#Variables that hold the object files for each part of the project
A_OBJS = hospitalDatabase.o
B_OBJS = PatientCSV.o
C_OBJS = PatientDML.o
D_OBJS = DoctorCSV.o
E_OBJS = DoctorDML.o
F_OBJS = AppointmentsCSV.o
G_OBJS = AppointmentsDML.o
H_OBJS = MainMenu.o
I_OBJS = EmergencyContactsCSV.o
J_OBJS = TestResultsCSV.o
M_OBJS = TestResultsDML.o

#Target that builds all parts of the project
all: hospitalDatabase PatientCSV PatientDML DoctorCSV DoctorDML AppointmentsCSV AppointmentsDML MainMenu TestResultsCSV EmergencyContactsCSV TestResultsDML

#Target that builds only the database-related parts
database: hospitalDatabase PatientCSV DoctorCSV AppointmentsCSV TestResultsCSV EmergencyContactsCSV

#Target that builds only the program-related parts 
#(Main Menu only one being made as the Main Menu program handles the creation of other files as they are used)
program: MainMenu

#Targets for building each part of the project. 
#They depend on their respective object files and are built using the compiler with the specified flags and libraries.
hospitalDatabase: $(A_OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

PatientCSV: $(B_OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

PatientDML: $(C_OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

DoctorCSV: $(D_OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

DoctorDML: $(E_OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

AppointmentsCSV: $(F_OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

AppointmentsDML: $(G_OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

MainMenu: $(H_OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

EmergencyContactsCSV: $(I_OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

TestResultsCSV: $(J_OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

TestResultsDML: $(M_OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)


#Target to remove all generated object files and executables.
clean:
	rm -f *.o hospitalDatabase PatientCSV PatientDML DoctorCSV DoctorDML AppointmentsCSV AppointmentsDML MainMenu TestResultsCSV EmergencyContactsCSV TestResultsDML EmergencyContactsDML
