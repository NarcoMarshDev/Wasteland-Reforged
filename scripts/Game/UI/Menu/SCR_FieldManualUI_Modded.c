modded class SCR_FieldManualUI
{
	protected IEntity m_StoreReference;
	protected WR_StoreType m_StoreType;
	protected PlayerController m_LocalPlayerController;
	
	protected override override void OnMenuOpen()
	{
		// ---------------- Modded start ---------------- //
		m_LocalPlayerController = GetGame().GetPlayerController();
		// get reference to store local player is currently using
		m_StoreReference = WR_CurrentStorePlayerControllerComponent.Cast( m_LocalPlayerController.FindComponent(WR_CurrentStorePlayerControllerComponent) ).GetCurrentStoreEntity();
		if (!m_StoreReference)
		{
			Print("No valid store reference found", LogLevel.WARNING);
			Close();
			return;
		}
		// get store type
		m_StoreType = WR_StoreComponent.Cast( m_StoreReference.FindComponent(WR_StoreComponent) ).GetStoreType();
		
		Print("Store Ref: " + m_StoreReference);
		Print("Store Type: " + m_StoreType);
		// working!
		
		// now we have a reference to the store currently being accessed an it's type, we need to load the appropriate config
		
		// ----------------- Modded end ----------------- //
		
		Widget rootWidget = GetRootWidget();
		SCR_ConfigUIComponent component = SCR_ConfigUIComponent.Cast(rootWidget.FindHandler(SCR_ConfigUIComponent));
		if (!component)
		{
			Print("No config component found | " + __FILE__ + ": " + __LINE__, LogLevel.WARNING);
			Close();
			return;
		}

		SCR_FieldManualConfigRoot configRoot = SCR_FieldManualConfigLoader.LoadConfigRoot(component.m_ConfigPath);
		if (!configRoot)
		{
			Print("Could not load provided config | " + __FILE__ + ": " + __LINE__, LogLevel.WARNING);
			Close();
			return;
		}

		m_ConfigRoot = configRoot;
		SetAllEntriesAndParents();

		// which background do you want, gents?
		bool isMainMenu = GetGame().m_bIsMainMenuOpen;
		Widget blur = GetRootWidget().FindAnyWidget("Blur");
		Widget backgroundImage = GetRootWidget().FindAnyWidget("MenuBackground");
		if (blur && isMainMenu)
			blur.RemoveFromHierarchy();
		if (backgroundImage && !isMainMenu)
		    backgroundImage.RemoveFromHierarchy();

		Widget menuFrame = rootWidget.FindAnyWidget("MenuFrame");
		TextWidget menuTitle = TextWidget.Cast(menuFrame.FindAnyWidget("Title"));
		VerticalLayoutWidget menuCategoryList = VerticalLayoutWidget.Cast(rootWidget.FindAnyWidget("CategoryList"));
		GridLayoutWidget menuGridLayout = GridLayoutWidget.Cast(rootWidget.FindAnyWidget("GridLayout"));
		if (!menuFrame || !menuTitle || !menuCategoryList || !menuGridLayout)
		{
			Print("Important elements (menu frame & title, category list, grid layout) are missing | " + __FILE__ + ": " + __LINE__, LogLevel.WARNING);
			Close();
			return;
		}

		Widget tabViewRoot = menuFrame.FindAnyWidget("TabViewRoot0");
		if (!tabViewRoot)
		{
			Print("no tab view root | " + FilePath.StripPath(__FILE__) + ":" + __LINE__, LogLevel.WARNING);
			Close();
			return;
		}

		SCR_TabViewComponent menuTabViewComponent = SCR_TabViewComponent.Cast(tabViewRoot.FindHandler(SCR_TabViewComponent));
		if (!menuTabViewComponent)
		{
			Print("no menu tab view component | " + FilePath.StripPath(__FILE__) + ":" + __LINE__, LogLevel.WARNING);
			Close();
			return;
		}

		Widget readingWidget = rootWidget.FindAnyWidget("ReadingWidget");

		TextWidget entryTitle = TextWidget.Cast(readingWidget.FindAnyWidget("EntryTitle"));
		Widget entryFrame = readingWidget.FindAnyWidget("EntryFrame");
		if (!readingWidget || !entryTitle || !entryFrame)
		{
			Print(string.Format("missing one: reading widget (%1), entryTitle (%2), entryFrame (%3) | " + FilePath.StripPath(__FILE__) + ":" + __LINE__, readingWidget != null, entryTitle != null, entryFrame != null), LogLevel.WARNING);
			Close();
			return;
		}

		SCR_NavigationButtonComponent backButtonMenuFrame = SCR_NavigationButtonComponent.GetNavigationButtonComponent("Back", menuFrame);
		if (!backButtonMenuFrame || !backButtonMenuFrame.m_OnClicked)
		{
			Print(string.Format("missing menu frame's back button menu (%1) or m_OnClicked (%2) | " + FilePath.StripPath(__FILE__) + ":" + __LINE__, backButtonMenuFrame != null, backButtonMenuFrame.m_OnClicked != null), LogLevel.WARNING);
			Close();
			return;
		}

		m_MenuBtnBack = backButtonMenuFrame;

		m_wMenuTitle = menuTitle;
		m_MenuTabView = menuTabViewComponent;
		m_wMenuCategoryList = menuCategoryList;
		m_wMenuGridLayout = menuGridLayout;

		m_wReadingWidget = readingWidget;
		m_wPageEntryTitle = entryTitle;
		m_wPageFrame = entryFrame;

		// breadcrumbs is not mandatory
		m_BreadCrumbsComponent = SCR_BreadCrumbsComponent.Cast(SCR_BreadCrumbsComponent.GetComponent(SCR_BreadCrumbsComponent, "BreadCrumbs", rootWidget));

		// scrolling reset is not mandatory
		m_wGridScrollLayoutWidget = ScrollLayoutWidget.Cast(rootWidget.FindAnyWidget("GridScrollLayout"));

		// searchbar functionality is not mandatory
		m_MenuSearchbar = SCR_EditBoxSearchComponent.Cast(SCR_EditBoxSearchComponent.GetEditBoxComponent("Searchbar", rootWidget));
		if (m_MenuSearchbar)
		{
			if (m_MenuSearchbar.m_OnConfirm)
				m_MenuSearchbar.m_OnConfirm.Insert(ProcessSearch);
		}

		m_MenuBtnBack.m_OnActivated.Insert(CloseMenuOrReadingPanel);

#ifdef WORKBENCH
		InputManager inputManager = GetGame().GetInputManager();
		if (inputManager)
		{
			inputManager.AddActionListener("MenuOpenWB", EActionTrigger.DOWN, CloseMenuOrReadingPanel);
			inputManager.AddActionListener("MenuBackWB", EActionTrigger.DOWN, CloseMenuOrReadingPanel);
		}
#endif

		m_SubCategoryButtonEventHandler = new SCR_FieldManualSubCategoryScriptedWidgetEventHandler(this);
		m_EntryButtonEventHandler = new SCR_FieldManualEntryScriptedWidgetEventHandler(this);
	}
}