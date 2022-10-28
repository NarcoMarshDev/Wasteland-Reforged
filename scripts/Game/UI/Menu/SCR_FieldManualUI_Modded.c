// HUGE TODO
// This whole store ui based on the field manual needs to be completely rewritten to be portable to arma 4

modded class SCR_FieldManualUI : MenuRootBase
{
	protected TextWidget m_wMenuTitle;
	protected SCR_TabViewComponent m_MenuTabView;
	protected VerticalLayoutWidget m_wMenuCategoryList;
	protected SCR_EditBoxSearchComponent m_MenuSearchbar;
	protected SCR_NavigationButtonComponent m_MenuBtnBack;

	protected Widget m_wReadingWidget;
	protected SCR_BreadCrumbsComponent m_BreadCrumbsComponent;
	protected TextWidget m_wPageEntryTitle;
	protected Widget m_wPageFrame;
	protected ScrollLayoutWidget m_wGridScrollLayoutWidget;
	protected GridLayoutWidget m_wMenuGridLayout;

	protected string m_sLastSearch;

	protected ref SCR_FieldManualConfigRoot m_ConfigRoot;
	protected ref array<ref SCR_FieldManualConfigEntry> m_aAllEntries;
	protected ref map<Widget, ref SCR_FieldManualConfigEntry> m_mWidgetEntryMap;
	protected ref map<Widget, ref SCR_FieldManualConfigCategory> m_mWidgetSubCategoryMap;
	protected ref SCR_FieldManualSubCategoryScriptedWidgetEventHandler m_SubCategoryButtonEventHandler;
	protected ref SCR_FieldManualEntryScriptedWidgetEventHandler m_EntryButtonEventHandler;
	protected Widget m_wLastClickedSubCategory;

	protected SCR_FieldManualConfigEntry m_CurrentEntry;

	protected ButtonWidget m_wFirstSubCategoryButton;

	protected bool m_bIsInEntryViewMode;
	protected bool m_bIsInSearchMode;
	protected bool m_bOpenedFromOutside;

	protected ResourceName m_ConfigPath;

	protected static const int S_TILES_GRID_WIDTH = 5;
	protected static const ResourceName S_ENTRY_TITLE_LAYOUT = "{B3656334DE3D701F}UI/layouts/WidgetLibrary/TextWidgets/Text_Heading3.layout";
	protected static const string S_SEARCH_RESULT_KEY = "#AR-FieldManual_SearchResult";
	protected static const string S_SEARCH_NO_RESULT_KEY = "#AR-FieldManual_SearchResult_None";

	protected bool m_bArmaWarning;
	// Modded
	protected IEntity m_StoreReference;
	protected WR_StoreType m_StoreType;
	protected PlayerController m_LocalPlayerController;
	protected float m_fPriceScaleFactor = 1;

	//------------------------------------------------------------------------------------------------
	void SCR_FieldManualUI()
	{
		m_aAllEntries = {};
		m_mWidgetEntryMap = new map<Widget, ref SCR_FieldManualConfigEntry>();
		m_mWidgetSubCategoryMap = new map<Widget, ref SCR_FieldManualConfigCategory>();
	}

	//------------------------------------------------------------------------------------------------
	protected override override void OnMenuOpen()
	{
		// ---------------- Modded start ---------------- //
		m_LocalPlayerController = GetGame().GetPlayerController();
		// get reference to store local player is currently using
		m_StoreReference = WR_CurrentStorePlayerControllerComponent.Cast( m_LocalPlayerController.FindComponent(WR_CurrentStorePlayerControllerComponent) ).GetCurrentStoreEntity();
		// get store type
		if (m_StoreReference)
		{
			m_StoreType = WR_StoreComponent.Cast( m_StoreReference.FindComponent(WR_StoreComponent) ).GetStoreType();
		}
		else
		{
			m_StoreType = null;
		}
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
			// modded, change menu title depending on store type, use normal field manual title if no store
		if (m_StoreReference)
		{
			string title;
			switch (m_StoreType)
			{
				case WR_StoreType.GunStore:
					title = "Gun Store";
					break;
				
				case WR_StoreType.VehicleStore:
					title = "Vehicle Store";
					break;
				
				case WR_StoreType.GeneralStore:
					title = "General Store";
					break;
				
				default:
					title = "Unknown Store";
					break;
			}
			menuTitle.SetText(title);
		}
		//m_wMenuTitle.SetText(m_ConfigRoot.m_sTitle);

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
	
	//------------------------------------------------------------------------------------------------
	protected override void OnMenuClosed()
	{
		WR_CurrentStorePlayerControllerComponent.Cast( m_LocalPlayerController.FindComponent(WR_CurrentStorePlayerControllerComponent) ).SetCurrentStoreEntity(null);
	}

	//------------------------------------------------------------------------------------------------
	protected override override void OnMenuShow()
	{
		CreateCategoryMenuWidgets();
		CloseReadingPanel();
		OpenFirstSubCategory();
	}

	//------------------------------------------------------------------------------------------------
	override void OnSubCategoryClicked(Widget w)
	{
		bool resetTiles = m_bIsInSearchMode || m_bIsInEntryViewMode || w != m_wLastClickedSubCategory;
		m_bIsInSearchMode = false;
		m_wLastClickedSubCategory = w;
		CloseReadingPanel();
		ResetLastSearch();
		if (resetTiles)
			SetTilesByWidget(w);
	}

	//------------------------------------------------------------------------------------------------
	override void OnTileClicked(Widget w)
	{
		m_bIsInSearchMode = false;
		SetCurrentEntryByWidget(w);
		Print("Tile Clicked! " + w);
	}

	//------------------------------------------------------------------------------------------------
	//! creates a tile and sets its image, background & text
	protected override Widget CreateTileWidget(notnull SCR_FieldManualConfigEntry entry, notnull Widget parent)
	{
		Widget createdWidget = GetGame().GetWorkspace().CreateWidgets(m_ConfigRoot.m_MenuEntryTileLayout, parent);
		if (!createdWidget)
		{
			Print("could not create tile widget for entry | " + FilePath.StripPath(__FILE__) + ":" + __LINE__, LogLevel.WARNING);
			return null;
		}

		Widget assetImage = createdWidget.FindAnyWidget("AssetImage");
		if (assetImage)
		{
			ImageWidget backgroundImageWidget = ImageWidget.Cast(createdWidget.FindAnyWidget("FullBackground"));
			if (backgroundImageWidget && m_ConfigRoot.m_aTileBackgrounds && m_ConfigRoot.m_aTileBackgrounds.Count())
				backgroundImageWidget.LoadImageTexture(0, m_ConfigRoot.m_aTileBackgrounds.Get(Math.RandomInt(0, m_ConfigRoot.m_aTileBackgrounds.Count())));

			ImageWidget imageWidget = ImageWidget.Cast(assetImage.FindAnyWidget("Image"));
			if (imageWidget)
			{
				if (!entry.m_Image.IsEmpty())
				{
					imageWidget.LoadImageTexture(0, entry.m_Image);
					// modded, change image colour to white so it's not darkened, don't do anything if not in store menu
					if (m_StoreReference)
					{
						imageWidget.SetColor(Color.White);
					}
				}
				else
				{
					imageWidget.SetVisible(false);
				}
			}
		}

		Widget assetInfo = createdWidget.FindAnyWidget("AssetInfo");
		if (assetInfo)
		{
			TextWidget textWidget = TextWidget.Cast(assetInfo.FindAnyWidget("EntityName"));
			if (textWidget)
				textWidget.SetText(entry.m_sTitle);
		}

		return createdWidget;
	}

	//------------------------------------------------------------------------------------------------
	//! sets entry tiles from a sub-category's widget
	// used in SCR_FieldManualSubCategoryScriptedWidgetEventHandler.OnClick
	protected override void SetTilesByWidget(Widget widget)
	{
		SCR_FieldManualConfigCategory subCategory = m_mWidgetSubCategoryMap.Get(widget);

		if (!subCategory)
			return;

		if (m_BreadCrumbsComponent && subCategory.m_Parent)
			m_BreadCrumbsComponent.Set(subCategory.m_Parent.m_sTitle, subCategory.m_sTitle);

		SetTiles(subCategory.m_aEntries);
	}

	//------------------------------------------------------------------------------------------------
	//! sets main Field Manual grid tiles from entries
	protected override void SetTiles(array<ref SCR_FieldManualConfigEntry> entries)
	{
		if (!m_wMenuGridLayout)
			return;

		SCR_WidgetHelper.RemoveAllChildren(m_wMenuGridLayout);

//		if (m_wGridScrollLayoutWidget)
//			m_wGridScrollLayoutWidget.SetSliderPosPixels(0, 0);

		if (!entries || entries.Count() < 1)
			return;

		m_mWidgetEntryMap.Clear();

		int column, line;
		Widget assetCard;
		ButtonWidget button;
		foreach (SCR_FieldManualConfigEntry entry : entries)
		{
			assetCard = CreateTileWidget(entry, m_wMenuGridLayout);
			if (!assetCard)
				continue;

			GridSlot.SetColumn(assetCard, column);
			GridSlot.SetRow(assetCard, line);

			column++;
			if (column >= S_TILES_GRID_WIDTH)
			{
				column = 0;
				line += 1;
			}

			button = ButtonWidget.Cast(SCR_WidgetHelper.GetWidgetOrChild(assetCard, "AssetCard0"));
			if (button)
			{
				button.AddHandler(m_EntryButtonEventHandler);
				m_mWidgetEntryMap.Insert(button, entry);
			}
		}
	}
	
	// temp method for printing hierarchy of widget
	// #ESE_ADD
	protected void PrintChildHierarchy(Widget w, string depthStr="")
	{
		if (depthStr == "")
		{
			Print("----------------------------------");
			Print("Printing Widget Child Hierarchy...");
			Print(" ");
			w = w.GetChildren();
		}
		int depth = depthStr.Length() / 3;
		depthStr = depthStr + " - ";
		while (w)
		{
			Print("" + depth + depthStr + w);
			Widget child = w.GetChildren();
			if (child)
			{
				PrintChildHierarchy(child, depthStr);
			}
			w = w.GetSibling();
		}
	}
	
	SCR_FieldManualConfigEntry GetCurrentEntry()
	{
		return m_CurrentEntry;
	}
	
	//------------------------------------------------------------------------------------------------
	//! sets the current entry to read - can be null (leave reading mode) or any SCR_FieldManualConfigEntry
	protected override void SetCurrentEntry(SCR_FieldManualConfigEntry entry)
	{
		if (!m_wPageFrame || !m_wPageEntryTitle)
		{
			Print("No page frame or page entry title!", LogLevel.WARNING);
			return;
		}
		
		SCR_WidgetHelper.RemoveAllChildren(m_wPageFrame);

		m_bIsInEntryViewMode = entry != null;

		m_wGridScrollLayoutWidget.SetVisible(!m_bIsInEntryViewMode);
		m_wReadingWidget.SetVisible(m_bIsInEntryViewMode); // change to m_wReadingWidget.SetVisible(m_bIsInEntryViewMode); to revert changes
		// modded
		Print(m_wGridScrollLayoutWidget);
		Print(m_wReadingWidget);
		
		// modded, remove reading widget background if in store menu
		if (m_StoreReference)
		{
			//PrintChildHierarchy(m_wReadingWidget);
		
			ImageWidget bgImage = ImageWidget.Cast( m_wReadingWidget.FindAnyWidget("EntryBackground") );
			if (bgImage)
			{
				bgImage.SetVisible(false);
			}
			ImageWidget bgGradient = ImageWidget.Cast( m_wReadingWidget.FindAnyWidget("BackgroundGradient") );
			if (bgGradient)
			{
				bgGradient.SetVisible(false);
			}
			ButtonWidget purchaseButton = ButtonWidget.Cast( m_wReadingWidget.FindAnyWidget("ConfirmPurchaseButton") );
			if (purchaseButton)
			{
				purchaseButton.SetVisible(true);
			}
		}
		if (!m_bIsInEntryViewMode)
		{
			if (m_BreadCrumbsComponent && m_CurrentEntry && m_CurrentEntry.m_Parent && m_CurrentEntry.m_Parent.m_Parent)
				m_BreadCrumbsComponent.Set(m_CurrentEntry.m_Parent.m_Parent.m_sTitle, m_CurrentEntry.m_Parent.m_sTitle);

			m_CurrentEntry = null;
			return;
		}

		if (m_BreadCrumbsComponent && entry.m_Parent && entry.m_Parent.m_Parent)
			m_BreadCrumbsComponent.Set(entry.m_Parent.m_Parent.m_sTitle, entry.m_Parent.m_sTitle, entry.m_sTitle);

		m_CurrentEntry = entry;
		
		m_wPageEntryTitle.SetText(entry.m_sTitle);

		// entry tab management - disabled for now
/*
		if (m_PageTabView)
		{
			while (m_PageTabView.GetTabCount() > 0)
			{
				m_PageTabView.RemoveTab(0);
			}
			m_PageTabView.AddTab("", entry.m_sTitle);
			m_PageTabView.ShowTab(0, false, false);
		}
// */

		Widget readingWidget = entry.CreateWidget(m_wPageFrame);
		// modded, change entry background if in store menu
		if (m_StoreReference)
		{
			
		}
		SCR_FieldManualConfigEntry_Weapon weaponEntry = SCR_FieldManualConfigEntry_Weapon.Cast(entry);
		if (weaponEntry)
			FillEntry_Weapon(readingWidget, weaponEntry);
	}

	//------------------------------------------------------------------------------------------------
	//! Opens an entry "from outside", e.g a hint
	override void OpenEntry(EFieldManualEntryId entryId)
	{
		if (entryId == EFieldManualEntryId.NONE)
		{
			SetCurrentEntry(null);
			return;
		}

		foreach (SCR_FieldManualConfigEntry entry : m_aAllEntries)
		{
			if (entry.m_eId == entryId)
			{
				m_bOpenedFromOutside = true;
				SetCurrentEntry(entry);
				return;
			}
		}

		SetCurrentEntry(null);
	}

	//------------------------------------------------------------------------------------------------
	//! sets the read entry linked from that widget
	protected override void SetCurrentEntryByWidget(Widget widget)
	{
		SCR_FieldManualConfigEntry entry = m_mWidgetEntryMap.Get(widget);
		if (!entry)
			return;

		SetCurrentEntry(entry);
	}

	//------------------------------------------------------------------------------------------------
	protected override void FillEntry_Weapon(notnull Widget widget, notnull SCR_FieldManualConfigEntry_Weapon entry)
	{
		Widget weaponRender = widget.FindAnyWidget("weaponRender");
		if (true /* remove later */ || !FillEntry_Weapon_Render(RenderTargetWidget.Cast(weaponRender), entry.m_WeaponEntityPath))
			if (weaponRender)
				weaponRender.RemoveFromHierarchy();
		FillEntry_Weapon_Statistics(widget.FindAnyWidget("statsLayout"), entry);
	}

	//------------------------------------------------------------------------------------------------
	protected override bool FillEntry_Weapon_Render(RenderTargetWidget renderTarget, ResourceName weaponResourceName)
	{
		if (!renderTarget || !weaponResourceName)
			return false;

		Resource resource = Resource.Load(weaponResourceName);
		IEntity entity = GetGame().SpawnEntityPrefabLocal(resource, null, null);
		if (!entity)
			return false;

		ItemPreviewManagerEntity manager = GetGame().GetItemPreviewManager();
		if (!manager)
			return false;

		ItemPreviewWidget renderPreview = ItemPreviewWidget.Cast(renderTarget);
		if (!renderPreview)
			return false;

		manager.SetPreviewItem(renderPreview, entity);
		return true;
	}

	//------------------------------------------------------------------------------------------------
	protected override void FillEntry_Weapon_Statistics(Widget statsLayout, SCR_FieldManualConfigEntry_Weapon entry)
	{
		if (!statsLayout)
			return;

		SCR_FieldManualUI_WeaponStatsHelper weaponStatsHelper = entry.m_WeaponStatsHelper;
		if (!weaponStatsHelper)
		{
			SCR_WidgetHelper.RemoveAllChildren(statsLayout);
			return;
		}

		FillEntry_Weapon_Statistics_Weight(statsLayout, weaponStatsHelper);
		FillEntry_Weapon_Statistics_Zeroing(statsLayout, weaponStatsHelper);
		FillEntry_Weapon_Statistics_DefaultZeroing(statsLayout, weaponStatsHelper);
		FillEntry_Weapon_Statistics_RateOfFire(statsLayout, weaponStatsHelper);
		FillEntry_Weapon_Statistics_MuzzleVelocity(statsLayout, weaponStatsHelper);
		FillEntry_Weapon_Statistics_FireModes(statsLayout, weaponStatsHelper);
	}

	//------------------------------------------------------------------------------------------------
	protected override void FillEntry_Weapon_Statistics_Weight(notnull Widget statsLayout, notnull SCR_FieldManualUI_WeaponStatsHelper weaponStatsHelper)
	{
		SCR_FieldManual_StatisticsLineComponent statsWeight = SCR_FieldManual_StatisticsLineComponent.GetComponent(statsLayout, "statsWeight");
		if (!statsWeight || weaponStatsHelper.GetWeight() < 0)
		{
			if (statsWeight)
				statsWeight.RemoveWidgetFromHierarchy();
		}
		else
		{
			statsWeight.SetTranslatedValue("#AR-ValueUnit_Short_Kilograms", weaponStatsHelper.GetWeight().ToString(-1, 2));
		}
	}

	//------------------------------------------------------------------------------------------------
	protected override void FillEntry_Weapon_Statistics_Zeroing(notnull Widget statsLayout, notnull SCR_FieldManualUI_WeaponStatsHelper weaponStatsHelper)
	{
		SCR_FieldManual_StatisticsLineComponent statsZeroing = SCR_FieldManual_StatisticsLineComponent.GetComponent(statsLayout, "statsZeroing");
		if (!statsZeroing || !weaponStatsHelper.GetZeroing() || weaponStatsHelper.GetZeroing().IsEmpty())
		{
			if (statsZeroing)
				statsZeroing.RemoveWidgetFromHierarchy();
		}
		else
		{
			array<string> zeroingStr = {};
			array<int> zeroing = weaponStatsHelper.GetZeroing();
			for (int i = 0, size = zeroing.Count(); i < size; i++)
			{
				zeroingStr.Insert(WidgetManager.Translate("#AR-ValueUnit_Short_Meters", zeroing[i]));
			}
			statsZeroing.SetValue(ArrayJoin(zeroingStr));
		}
	}

	//------------------------------------------------------------------------------------------------
	protected override void FillEntry_Weapon_Statistics_DefaultZeroing(notnull Widget statsLayout, notnull SCR_FieldManualUI_WeaponStatsHelper weaponStatsHelper)
	{
		SCR_FieldManual_StatisticsLineComponent statsDefaultZeroing = SCR_FieldManual_StatisticsLineComponent.GetComponent(statsLayout, "statsDefaultZeroing");
		if (!statsDefaultZeroing || weaponStatsHelper.GetDefaultZeroing() < 1)
		{
			if (statsDefaultZeroing)
				statsDefaultZeroing.RemoveWidgetFromHierarchy();
		}
		else
		{
			statsDefaultZeroing.SetTranslatedValue("#AR-ValueUnit_Short_Meters", weaponStatsHelper.GetDefaultZeroing().ToString());
		}
	}

	//------------------------------------------------------------------------------------------------
	protected override void FillEntry_Weapon_Statistics_RateOfFire(notnull Widget statsLayout, notnull SCR_FieldManualUI_WeaponStatsHelper weaponStatsHelper)
	{
		SCR_FieldManual_StatisticsLineComponent statsRateOfFire = SCR_FieldManual_StatisticsLineComponent.GetComponent(statsLayout, "statsRateOfFire");
		if (!statsRateOfFire || weaponStatsHelper.GetRateOfFire() < 1)
		{
			if (statsRateOfFire)
				statsRateOfFire.RemoveWidgetFromHierarchy();
		}
		else
		{
			statsRateOfFire.SetTranslatedValue("#AR-ValueUnit_Short_RoundsPerMinute", weaponStatsHelper.GetRateOfFire().ToString());
		}
	}

	//------------------------------------------------------------------------------------------------
	protected override void FillEntry_Weapon_Statistics_MuzzleVelocity(notnull Widget statsLayout, notnull SCR_FieldManualUI_WeaponStatsHelper weaponStatsHelper)
	{
		SCR_FieldManual_StatisticsLineComponent statsMuzzleVelocity = SCR_FieldManual_StatisticsLineComponent.GetComponent(statsLayout, "statsMuzzleVelocity");
		if (!statsMuzzleVelocity || weaponStatsHelper.m_iMuzzleVelocity < 1)
		{
			if (statsMuzzleVelocity)
				statsMuzzleVelocity.RemoveWidgetFromHierarchy();
		}
		else
		{
			statsMuzzleVelocity.SetTranslatedValue("#AR-ValueUnit_Short_MetersPerSeconds", weaponStatsHelper.m_iMuzzleVelocity.ToString());
		}
	}

	//------------------------------------------------------------------------------------------------
	protected override void FillEntry_Weapon_Statistics_FireModes(notnull Widget statsLayout, notnull SCR_FieldManualUI_WeaponStatsHelper weaponStatsHelper)
	{
		SCR_FieldManual_StatisticsLineComponent statsFireModes = SCR_FieldManual_StatisticsLineComponent.GetComponent(statsLayout, "statsFireModes");
		if (!statsFireModes || !weaponStatsHelper.GetFireModes() || weaponStatsHelper.GetFireModes().IsEmpty())
		{
			if (statsFireModes)
				statsFireModes.RemoveWidgetFromHierarchy();
		}
		else
		{
			statsFireModes.SetValue(ArrayJoin(weaponStatsHelper.GetFireModes()));
		}
	}

	//------------------------------------------------------------------------------------------------
	//! one-time call that goes through all categories and entries and adds enabled entries to a one-level array
	// to do: breakdown in smaller methods?
	
	// modded, changes the category removal check depending on if we are at a store or not
	protected bool CategoryConditional(SCR_FieldManualConfigCategory category, map<string, WR_StoreType> storeTypeMap)
	{
		string title = category.m_sTitle;
		if (m_StoreReference)
		{
			Print(storeTypeMap.Get(title));
			Print(m_StoreType);
			if (!category.m_bEnabled || storeTypeMap.Get(title) != m_StoreType)
			{
				return true;
			}
			return false;
		}
		else
		{
			if (!category.m_bEnabled || title.Contains("Gun Store") || title.Contains("Vehicle Store") || title.Contains("General Store"))
			{
				return true;
			}
			return false;
		}
	}
	
	protected override void SetAllEntriesAndParents()
	{
		m_aAllEntries.Clear();
		array<SCR_FieldManualConfigCategory> categoriesToRemove = {};
		array<SCR_FieldManualConfigCategory> subCategoriesToRemove = {};
		array<SCR_FieldManualConfigEntry> entriesToRemove = {};
		// modded, create map linking store type enum to config category names
		map<string, WR_StoreType> storeTypeMap = new map<string, WR_StoreType>();
		storeTypeMap.Insert("Gun Store", WR_StoreType.GunStore);
		storeTypeMap.Insert("Vehicle Store", WR_StoreType.VehicleStore);
		storeTypeMap.Insert("General Store", WR_StoreType.GeneralStore);
		
		foreach (SCR_FieldManualConfigCategory category : m_ConfigRoot.m_aCategories)
		{
			// modded, runs conditional function to check valid categories depending on if we are at a store or not
			if (CategoryConditional(category, storeTypeMap))
			{
				categoriesToRemove.Insert(category);
				continue;
			}
			// working!
			
			subCategoriesToRemove.Clear();
			foreach (SCR_FieldManualConfigCategory subCategory : category.m_aCategories) // only two levels are supported by the UI for now
			{
				if (!subCategory.m_bEnabled)
				{
					subCategoriesToRemove.Insert(subCategory);
					continue;
				}

				entriesToRemove.Clear();
				foreach (SCR_FieldManualConfigEntry entry : subCategory.m_aEntries)
				{
					//if (!entry.m_bEnabled || entry.m_aContent.IsEmpty())
					// modded, show all entries no matter if they're empty or not
					if (!entry.m_bEnabled)
					{
						entriesToRemove.Insert(entry);
						continue;
					}
					entry.m_Parent = subCategory;
					// modded, combines title and price value to display as one string if we are at a store
					if (m_StoreReference)
					{
						WR_FieldManualConfigEntry_Store newEntry = WR_FieldManualConfigEntry_Store.Cast(entry);
						if (!newEntry)
						{
							continue;
						}
						newEntry.m_iPrice = ESE_Math.CeilToNearestMultiple(newEntry.m_iPrice * m_fPriceScaleFactor, 10);
						newEntry.m_sTitle = newEntry.m_sTitle + "\nPrice: " + newEntry.m_iPrice;
						m_aAllEntries.Insert(entry);
						continue;
					}
					m_aAllEntries.Insert(entry);
				}
				foreach (SCR_FieldManualConfigEntry entry : entriesToRemove)
				{
					subCategory.m_aEntries.RemoveItemOrdered(entry);
				}

				if (subCategory.m_aEntries.IsEmpty() && subCategory.m_aCategories.IsEmpty())
				{
					subCategoriesToRemove.Insert(subCategory);
					continue;
				}

				subCategory.m_Parent = category;
			}
			foreach (SCR_FieldManualConfigCategory subCategory : subCategoriesToRemove)
			{
				category.m_aCategories.RemoveItemOrdered(subCategory);
			}

			if (category.m_aEntries.IsEmpty() && category.m_aCategories.IsEmpty())
			{
				categoriesToRemove.Insert(category);
			}
		}
		foreach (SCR_FieldManualConfigCategory category : categoriesToRemove)
		{
			m_ConfigRoot.m_aCategories.RemoveItemOrdered(category);
		}
	}

	//------------------------------------------------------------------------------------------------
	//! creates main categories buttons list
	protected override void CreateCategoryMenuWidgets()
	{
		if (!m_ConfigRoot || !m_ConfigRoot.m_aCategories || !m_wMenuCategoryList)
			return;

		m_wFirstSubCategoryButton = null;
		m_mWidgetSubCategoryMap.Clear();
		SCR_WidgetHelper.RemoveAllChildren(m_wMenuCategoryList);
				
/*
		if (m_MenuTabView)
		{
			m_MenuTabView.AddTab("", m_ConfigRoot.m_sDefaultTabName);
			m_MenuTabView.AddTab("", m_ConfigRoot.m_sModsTabName);
			m_MenuTabView.ShowTab(0, false, false);
			m_MenuTabView.EnableTab(1, false, false);
		}
// */
		Widget subCategoryWidget;
		ButtonWidget button;
		foreach (SCR_FieldManualConfigCategory category : m_ConfigRoot.m_aCategories)
		{
			category.CreateWidget(m_wMenuCategoryList);

			foreach (SCR_FieldManualConfigCategory subCategory : category.m_aCategories)
			{
				subCategoryWidget = subCategory.CreateWidget(m_wMenuCategoryList);
				button = ButtonWidget.Cast(subCategoryWidget);
				if (button)
				{
					button.AddHandler(m_SubCategoryButtonEventHandler);
					m_mWidgetSubCategoryMap.Insert(button, subCategory);

					if (!m_wFirstSubCategoryButton)
						m_wFirstSubCategoryButton = button;
				}
			}
		}
	}

	//------------------------------------------------------------------------------------------------
	//! opens the first sub-category, showing its entries
	protected override void OpenFirstSubCategory()
	{
		OnSubCategoryClicked(m_wFirstSubCategoryButton);
		GetGame().GetCallqueue().CallLater(GetGame().GetWorkspace().SetFocusedWidget, 0, false, m_wFirstSubCategoryButton, false);
	}

	//------------------------------------------------------------------------------------------------
	//! resets last search value
	// used in SCR_FieldManualSubCategoryScriptedWidgetEventHandler.OnClick
	protected override void ResetLastSearch()
	{
		m_sLastSearch = "";
	}

	//------------------------------------------------------------------------------------------------
	//! the search process itself - finds entries and set tiles
	protected override void ProcessSearch(SCR_EditBoxSearchComponent searchbox, string search)
	{
		search = search.Trim();

		if (search.IsEmpty())
		{
			ResetLastSearch();
//			if (m_bIsInSearchMode)
//				OpenFirstSubCategory();

			return;
		}

		CloseReadingPanel();

		if (search.Contains("ArmA") && !m_bArmaWarning)
		{
			m_bArmaWarning = true;

			if (m_BreadCrumbsComponent)
				m_BreadCrumbsComponent.Set("Arma - not Armed Assault, not ArmA, Arma");

			SCR_FieldManualConfigEntry_Standard ArmaEntry = new SCR_FieldManualConfigEntry_Standard();
			ArmaEntry.m_sTitle = "Arma!!";
			SCR_FieldManualPiece_Text ArmaPiece = new SCR_FieldManualPiece_Text();
			ArmaPiece.m_sText = "It is written <i>\"Arma\"</i>, <b>not</b> <i>\"ArmA\"</i> ! FTFY";
			ArmaEntry.m_aContent.Insert(ArmaPiece);
			SetCurrentEntry(ArmaEntry);

			EditBoxWidget searchEditBox = EditBoxWidget.Cast(GetRootWidget().FindAnyWidget("Searchbar").FindAnyWidget("EditBox"));
			string searchText = searchEditBox.GetText();
			searchText.Replace("ArmA", "Arma");
			searchEditBox.SetText(searchText);

			ResetLastSearch();

			return;
		}

		m_bIsInSearchMode = true;

		string originalSearch = search;
		search.ToLower();
		if (search == m_sLastSearch)
			return;

		m_sLastSearch = search;

		array<ref SCR_FieldManualConfigEntry> entries = {};

		if (!m_sLastSearch.IsEmpty())
		{
			array<int> indexList = {};

			array<string> allEntriesKeys = {};
			foreach (SCR_FieldManualConfigEntry entry : m_aAllEntries)
			{
				allEntriesKeys.Insert(entry.m_sTitle);
			}

			WidgetManager.SearchLocalized(m_sLastSearch, allEntriesKeys, indexList);

			foreach (int index : indexList)
			{
				entries.Insert(m_aAllEntries.Get(index));
			}
		}

		if (m_BreadCrumbsComponent)
		{
			if (entries.IsEmpty())
				m_BreadCrumbsComponent.SetRichFormat(S_SEARCH_NO_RESULT_KEY, originalSearch);
			else
				m_BreadCrumbsComponent.SetRichFormat(S_SEARCH_RESULT_KEY, originalSearch, entries.Count().ToString());
		}

		SetTiles(entries);
	}

	//------------------------------------------------------------------------------------------------
	//! closes the reading panel (sets the current entry to null)
	protected override void CloseReadingPanel()
	{
		SetCurrentEntry(null);
	}

	//------------------------------------------------------------------------------------------------
	//! determines what happens when ESC is pressed
	//! - if in reading mode, closes the reading panel
	//! - if in main view, closes the Field Manual
	protected override void CloseMenuOrReadingPanel()
	{
		if (m_bIsInEntryViewMode && !m_bOpenedFromOutside)
		{
			CloseReadingPanel();
		}
		else
		{
			Close();
		}
	}

	//------------------------------------------------------------------------------------------------
	//----- STATIC METHODS
	//------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------
	//! opens the first entry matching the EFieldManualEntryId enum value
	static override SCR_FieldManualUI Open(EFieldManualEntryId entryId)
	{
		SCR_FieldManualUI ui = SCR_FieldManualUI.Cast(GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.FieldManualDialog));
		ui.OpenEntry(entryId);
		return ui;
	}

	//------------------------------------------------------------------------------------------------
	//----- my little helper!
	//------------------------------------------------------------------------------------------------
	//! joins strings with a stringtable separator - from 0 to 5 entries
	protected static override string ArrayJoin(notnull array<string> entries)
	{
		int count = entries.Count();
		switch (count)
		{
			case 0: return "-";
			case 1: return entries[0];
			case 2: return WidgetManager.Translate("#AR-General_List_Short_2", entries[0], entries[1]);
			case 3: return WidgetManager.Translate("#AR-General_List_Short_3", entries[0], entries[1], entries[2]);
			case 4: return WidgetManager.Translate("#AR-General_List_Short_4", entries[0], entries[1], entries[2], entries[3]);
			case 5: return WidgetManager.Translate("#AR-General_List_Short_5", entries[0], entries[1], entries[2], entries[3], entries[4]);
		}
		Print(string.Format("Too many list entries to join as a string - missing #AR-General_List_Short_%1 translation?", count), LogLevel.WARNING);
		return WidgetManager.Translate("#AR-General_List_Short_5", entries[0], entries[1], entries[2], entries[3], entries[4]);
	}
};