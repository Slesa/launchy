#region License
//  HotKeyManager 1.0
//																																							*
//  Copyright (C) 2004  http://www.onyeyiri.co.uk
//  Coded by: Nnamdi Onyeyiri
//
//  This code may be used in any way you desire except for commercial use.
//  The code can be freely redistributed unmodified as long as all of the authors 
//  copyright notices remain intact, as long as it is not being sold for profit.
//  Although I have tried to ensure the stability of the code, I will not take 
//  responsibility for any loss, damages, or death it may cause.
//
//  This software is provided "as is" without any expressed or implied warranty.
#endregion

using System;
using System.Collections;
using System.Windows.Forms;
using System.Runtime.InteropServices;



namespace Onyeyiri
{
	#region HotKeyEventHandler
	/// <summary>
	/// Delegate for HotKeyManager events.
	/// </summary>
	public delegate void HotKeyEventHandler(int hotKeyId);
	#endregion
	
	#region HotKeyModifier
	/// <summary>
	/// Modifiers for hot keys.
	/// </summary>
	[Flags]
	public enum HotKeyModifier
	{
		/// <summary>
		/// Alt key.
		/// </summary>
		Alt = 0x0001,

		/// <summary>
		/// Control key.
		/// </summary>
		Control = 0x0002,

		/// <summary>
		/// Shift key.
		/// </summary>
		Shift = 0x0004,

		/// <summary>
		/// Windows key.
		/// </summary>
		Windows = 0x0008
	}
	#endregion

	#region HotKey
	/// <summary>
	/// Contains information on a hotkey.
	/// </summary>
	public class HotKey
	{
		#region Private Variables
		private int m_id;
		private HotKeyModifier m_modifier;
		private Keys m_key;
		private HotKeyEventHandler m_handler;
		#endregion

		#region Public Properties
		/// <summary>
		/// Gets the hotkey identifier.
		/// </summary>
		public int ID
		{
			get
			{
				return m_id;
			}
		}

		/// <summary>
		/// Gets the hotkeys modifiers.
		/// </summary>
		public HotKeyModifier Modifiers
		{
			get
			{
				return m_modifier;
			}
		}

		/// <summary>
		/// Gets the hotkeys key.
		/// </summary>
		public Keys Key
		{
			get
			{
				return m_key;
			}
		}

		/// <summary>
		/// Gets / Sets the hotkeys event handler.
		/// </summary>
		public HotKeyEventHandler Handler
		{
			get
			{
				return m_handler;
			}

			set
			{
				m_handler = value;
			}
		}
		#endregion

		#region Construction
		/// <summary>
		/// Creates a new HotKey object.
		/// </summary>
		/// <param name="key">Key.</param>
		/// <param name="modifiers">Key modifiers.</param>
		/// <param name="id">Hotkey ID.</param>
		/// <param name="handler">Hotkey event handler.</param>
		public HotKey(Keys key, HotKeyModifier modifiers, int id, HotKeyEventHandler handler)
		{
			m_id = id;
			m_modifier = modifiers;
			m_key = key;
			m_handler = handler;
		}
		#endregion
	}
	#endregion

	/// <summary>
	/// Manages the registering and unregistering of hotkeys.
	/// </summary>
	public class HotKeyManager
	{
		#region Imports
		[DllImport("user32.dll")]
		public static extern bool RegisterHotKey(IntPtr hWnd,int id,int fsModifiers,int vlc);
		[DllImport("user32.dll")]
		public static extern bool UnregisterHotKey(IntPtr hWnd, int id);
		#endregion



		#region Private Variables
		private HotKeyControl m_ctrl;
		private HotKeyCollection m_keys;
		#endregion

		#region Construction/Destruction
		/// <summary>
		/// Creates a new HotKeyManager object.
		/// </summary>
		public HotKeyManager()
		{
			// setup the control that recieves the hotkey msg.
			m_ctrl = new HotKeyControl();
			m_ctrl.HotKeyHit += new HotKeyEventHandler(HotKeyHitHandler);

			// init the collection.
			m_keys = new HotKeyCollection();
		}

		~HotKeyManager()
		{
			// unregister all hotkeys.
			foreach(HotKey hk in m_keys)
			{
				Unregister(hk.ID);
			}
		}
		#endregion

		#region Structures
		private class HotKeyControl : Control
		{
			#region Overrides
			protected override void WndProc(ref Message m)
			{
				// check if its WM_HOTKEY
				// WM_HOTKEY = 0x0312;
				if (m.Msg == 0x0312)
				{
					// it is, so fire the event.
					if (HotKeyHit != null) HotKeyHit(m.WParam.ToInt32());
				}

				base.WndProc(ref m);
			}
			#endregion

			#region Events
			public event HotKeyEventHandler HotKeyHit;
			#endregion
		}

		/// <summary>
		/// Collection for HotKey objects.
		/// </summary>
		public class HotKeyCollection : CollectionBase
		{
			#region Public Properties
			public HotKey this[int index]
			{
				get
				{
					return (HotKey)List[index];
				}
			}
			#endregion

			#region Construction
			/// <summary>
			/// Creates a new hotkey collection.
			/// </summary>
			public HotKeyCollection()
			{
			}
			#endregion

			#region Methods
			/// <summary>
			/// Adds a hotkey to the collection.
			/// </summary>
			/// <param name="key"></param>
			public void Add(HotKey key)
			{
				List.Add(key);
			}

			/// <summary>
			/// Removes the hotkey with the specified ID from the colleciton.
			/// </summary>
			/// <param name="id">Hotkey ID.</param>
			public void RemoveId(int id)
			{
				for(int i = 0; i < List.Count; i++)
				{
					if (((HotKey)List[i]).ID == id)
					{
						List.RemoveAt(id);
						break;
					}
				}
			}
			#endregion
		}
		#endregion

		#region Methods
		/// <summary>
		/// Registers a new hotkey.
		/// </summary>
		/// <param name="key">Key.</param>
		/// <param name="modifier">Key modifiers.</param>
		/// <param name="id">Hotkey ID.</param>
		/// <param name="handler">Hotkey event handler.</param>
		/// <returns>True if hotkey was registered, otherwise false.</returns>
		public bool Register(Keys key, HotKeyModifier modifier, int id, HotKeyEventHandler handler)
		{
			// attempt to register the hotkey.
			bool result = RegisterHotKey(m_ctrl.Handle, id, (int)modifier, (int)key);

			// add the hotkey to the collection.
			if (result) m_keys.Add(new HotKey(key, modifier, id, handler));

			return result;
		}

		/// <summary>
		/// Registers a new hotkey.
		/// </summary>
		/// <param name="hk">Hotkey to register.</param>
		/// <returns>True if hotkey was registered, otherwise false.</returns>
		public bool Register(HotKey hk)
		{
			// register the hotkey.
			return Register(hk.Key, hk.Modifiers, hk.ID, hk.Handler);
		}

		/// <summary>
		/// Unregisters the specified hotkey.
		/// </summary>
		/// <param name="id">Hotkey Id.</param>
		/// <returns>True if the hotkey was unregistered, otherwise false.</returns>
		public bool Unregister(int id)
		{
			// attempt to unregister the hotkey.
			bool result =  UnregisterHotKey(m_ctrl.Handle, id);

			// remove the hotkey from the collection.
			if (result) m_keys.RemoveId(id);

			return result;
		}

		/// <summary>
		/// Unregisters the specified hotkey.
		/// </summary>
		/// <param name="hk">Hotkey to unregister.</param>
		/// <returns>True if the hotkey was unregistered, otherwise false.</returns>
		public bool Unregister(HotKey hk)
		{
			// unregister the hotkey.
			return Unregister(hk.ID);
		}
		#endregion

		#region Private Methods
		private void HotKeyHitHandler(int id)
		{
			// pass the event on.
			foreach(HotKey hk in m_keys)
			{
				if (hk.ID == id)
				{
					// fire the event if its not null.
					if (hk.Handler != null)
					{
						hk.Handler(id);
					}
					else if (HotKeyHit != null)
					{
						// try managers event handler if individual one isnt set.
						HotKeyHit(id);
					}

					break;
				}
			}
		}
		#endregion

		#region Events
		/// <summary>
		/// Event fired when hotkey doesnt have its own event handler specified.
		/// </summary>
		public event HotKeyEventHandler HotKeyHit;
		#endregion
	}
}
