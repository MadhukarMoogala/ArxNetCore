// (C) Copyright 2002-2012 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("MAD")

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Threading::Tasks;
using namespace System::Drawing;
using namespace Autodesk::AutoCAD::ApplicationServices::Core;
using namespace Autodesk::AutoCAD::Internal;

// Part 1: AutoCAD Database Helper Class
//----------------------------------------
class AcDbHelper
{
public:
	// Smart pointer for AutoCAD database objects
	template <class T> struct unique_db_ptr : public std::unique_ptr<T, void(*)(AcDbObject*)>
	{
		unique_db_ptr<T>(T* t) : std::unique_ptr<T, void(*)(AcDbObject*)>(t, closeOrDeleteDbObj) { }

		static unique_db_ptr<T> create()
		{
			T* newObj = new T();
			return unique_db_ptr<T>(newObj);
		}
	};

	// Add entity to model space
	static bool addToDb(AcDbEntity* pEnt, AcDbDatabase* pDb = nullptr)
	{
		if (!pDb)
			pDb = acdbHostApplicationServices()->workingDatabase();

		unique_db_ptr<AcDbEntity> ent(pEnt);
		AcDbBlockTable* pBt;
		if (Acad::eOk != pDb->getBlockTable(pBt, AcDb::kForRead))
			return false;

		unique_db_ptr<AcDbBlockTable> bt(pBt);
		AcDbBlockTableRecord* pMs;
		if (Acad::eOk != pBt->getAt(ACDB_MODEL_SPACE, pMs, AcDb::kForWrite))
			return false;

		return Acad::eOk == unique_db_ptr<AcDbBlockTableRecord>(pMs)->appendAcDbEntity(ent.get());
	}

	// Create and add circle to database
	static bool createCircle(const AcGePoint3d& center, double radius, int colorIndex = 1)
	{
		auto circlePtr = unique_db_ptr<AcDbCircle>::create();
		if (!circlePtr)
			return false;

		AcDbCircle* circle = circlePtr.get();
		circle->setDatabaseDefaults();
		circle->setRadius(radius);
		circle->setColorIndex(colorIndex);
		circle->setCenter(center);

		return addToDb(circle);
	}

    // Generate a random color index

    static int getRandomColorIndex()
    {
        return (rand() % 256);
    }

private:
	// Helper function for smart pointer cleanup
	static void closeOrDeleteDbObj(AcDbObject* pObj)
	{
		if (pObj->objectId().isNull())
			delete pObj;
		else
			pObj->close();
	}
};

// Part 2: UI Form Implementation with Clean Design
//----------------------------------------

namespace UIForms {
	namespace AcadApp = Autodesk::AutoCAD::ApplicationServices::Core;
	namespace AcadUtils = Autodesk::AutoCAD::Internal;
	public ref class MainForm : public Form
	{
	private:
		
		// UI Controls
		Button^ drawButton;
		NumericUpDown^ radiusInput;
		Label^ radiusLabel;

        // Initialize the form controls
		void InitializeComponent()
		{
			this->Text = "Advanced AutoCAD Drawing Tool";
			this->Width = 400;
			this->Height = 200;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->StartPosition = FormStartPosition::CenterScreen;

			// Radius input setup
			radiusLabel = gcnew Label();
			radiusLabel->Text = "Circle Radius:";
			radiusLabel->Location = Point(50, 40);
			radiusLabel->AutoSize = true;

			radiusInput = gcnew NumericUpDown();
			radiusInput->Location = Point(150, 38);
			radiusInput->Minimum = 1;
			radiusInput->Maximum = 1000;
			radiusInput->Value = 10;
			radiusInput->DecimalPlaces = 2;

			// Draw button setup
			drawButton = gcnew Button();
			drawButton->Text = "Draw Circle";
			drawButton->Location = Point(150, 80);
			drawButton->Width = 100;
			drawButton->Click += gcnew EventHandler(this, &MainForm::DrawButton_Click);

			// Add controls to form
			this->Controls->Add(radiusLabel);
			this->Controls->Add(radiusInput);
			this->Controls->Add(drawButton);
			

		}
		// Helper method that performs the drawing operation on the main thread
		Task^ DrawCircleAsync(System::Object^ data)
		{
			// Create a TaskCompletionSource to manage the task lifecycle
			auto tcs = gcnew TaskCompletionSource();

			try
			{
				double radius = Convert::ToDouble(radiusInput->Value);
                int colorIndex = AcDbHelper::getRandomColorIndex();
				if (AcDbHelper::createCircle(AcGePoint3d::kOrigin, radius,colorIndex))
				{
					tcs->SetResult();
				}
				else
				{
					throw gcnew Exception("Failed to create circle");
				}
			}
			catch (System::Exception^ ex)
			{
				tcs->SetException(ex);
			}

			return tcs->Task;
		}
        void DrawButton_Click(System::Object^ sender, System::EventArgs^ e)
        {
            // Disable the button to prevent multiple clicks
            drawButton->Enabled = false;

            try
            {
                // Get the DocumentManager
                auto dm = Autodesk::AutoCAD::ApplicationServices::Core::Application::DocumentManager;

                // Create the delegate with the correct syntax
                auto callback = gcnew Func<Object^, Task^>(this, &MainForm::DrawCircleAsync);

                // Execute the callback in the command context
                auto task = dm->ExecuteInCommandContextAsync(callback, nullptr);
				task->GetResult();
                // Enable the button
				drawButton->Enabled = true;
                // Set focus to the drawing area and zoom extents
				AcadUtils::Utils::SetFocusToDwgView();
				AcadUtils::Utils::CancelAndRunCmds("_.zoom\n_extents\n");
            }
            catch (System::Exception^ ex)
            {
                MessageBox::Show("Error: " + ex->Message, "Error",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }		
	public:
		MainForm() {
			InitializeComponent();
		}
	};
}



// Part 3: ObjectARX Entry Point
//----------------------------------------
class CArxNetCoreApp : public AcRxArxApp {
public:
	CArxNetCoreApp() : AcRxArxApp() {}

	virtual AcRx::AppRetCode On_kInitAppMsg(void* pkt) {
		return AcRxArxApp::On_kInitAppMsg(pkt);
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg(void* pkt) {
		return AcRxArxApp::On_kUnloadAppMsg(pkt);
	}

	virtual void RegisterServerComponents() {
	}

	static void MADGUIToolLaunch() {
		try
		{
			auto form = gcnew UIForms::MainForm();
			Autodesk::AutoCAD::ApplicationServices::Application::ShowModelessDialog(form);
		}
		catch (System::Exception^ ex)
		{
			acutPrintf(L"\nException occurred: %s", ex->Message);
		}
	}
};

IMPLEMENT_ARX_ENTRYPOINT(CArxNetCoreApp)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxNetCoreApp, MADGUI, ToolLaunch, ToolLaunch, ACRX_CMD_MODAL, NULL)

